#include <functional>
#include <vector>
#include <cstdlib>
#include <numeric>

using u32 = uint_least32_t; 

//Initialization Operators ----------------------------------------------------------

/*
    Random Initialization: Randomly initialize a gene with chromosomes from chromosome_list
    Arguments:
        - size:                 number of chromosomes in gene
        - chromosome_list:      list of possible values for each chromosome
*/

std::function<std::vector<int>(std::mt19937&)> initialize_random(int size, const std::vector<int> chromosome_list) {
    return [size, chromosome_list](std::mt19937& generator) -> std::vector<int> {
        std::vector<int> gene;
        std::uniform_int_distribution< u32 > distribute_chromosome(0, chromosome_list.size() - 1);
        for (int i = 0; i < size; i++) {
            gene.emplace_back(chromosome_list[distribute_chromosome(generator)]);
        }
        return gene;
    };
}

/*
    Random Permutation Initialization: Randomly initialize a gene with a permutation of chromosome_list
    Arguments:
        - chromosome_list:      elements of permutation
*/

std::function<std::vector<int>(std::mt19937&)> initialize_random_permutation(const std::vector<int> chromosome_list) {
    return [chromosome_list](std::mt19937& generator) -> std::vector<int> {
        std::vector<int> gene = chromosome_list;
        std::shuffle(gene.begin(), gene.end(), generator);
        return gene;
    };
}

// Mutation Operators ---------------------------------------------------------------

/*  
    Numeric Mutation: Increment or decrement one bit by one
    Arguments:
        - mutation_rate:        overall probability of a mutation occurring
        - incrementation_rate:  probability of the mutation being an incrementation
        - chromosome_max:       maximum value for a chromosome
        - chromosome_min:       minimum value for a chromosome
*/

std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)> mutate_numeric(double mutation_rate, double incrementation_rate, int chromosome_max, int chromosome_min) {
    return [mutation_rate, incrementation_rate, chromosome_max, chromosome_min](const std::vector<int>& gene, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> mutated_gene = gene;
        std::uniform_real_distribution< double > distribute_rate(0, 1);
        if (distribute_rate(generator) < mutation_rate) {
            std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 1 );
            int chromosome = distribute_point(generator);
            if (distribute_rate(generator) < incrementation_rate) {
                mutated_gene[chromosome] = std::min(chromosome_max, mutated_gene[chromosome] + 1);
            } else {
                mutated_gene[chromosome] = std::max(chromosome_min, mutated_gene[chromosome] - 1);
            }
        }
        return mutated_gene;
    };
}

/*  
    Swap Mutation: Swap two bits
    Arguments:
        - mutation_rate:        overall probability of a mutation occurring
*/

std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)> mutate_swap(double mutation_rate) {
    return [mutation_rate](const std::vector<int>& gene, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> mutated_gene = gene;
        std::uniform_real_distribution< double > distribute_rate(0, 1);
        if (distribute_rate(generator) < mutation_rate) {
            std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 1 );
            int chromosome1 =  distribute_point(generator);
            int chromosome2 =  distribute_point(generator);
            mutated_gene[chromosome1] = gene[chromosome2];
            mutated_gene[chromosome2] = gene[chromosome1];
        }
        return mutated_gene;
    };
}

// Parent Selection Operators -------------------------------------------------------

/*
    Tournament Parent Selection: Take a random subgroup of a specified size and choose the one with the highest fitness value
    Arguments:
        - tournament_size: size of the chosen subgroup
*/

std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&, std::mt19937&)> select_tournament(int tournament_size) {
    return [tournament_size](const std::vector<double>& fitnesses, const std::vector<std::vector<int>>& genes, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> selected_genes(tournament_size);
        std::uniform_int_distribution< u32 > distribute_point(0, genes.size() - 1 );
        for (int i = 0; i < tournament_size; i++) {
            int rand_index = distribute_point(generator);
            selected_genes[i] = rand_index;
        }
        auto max_it = std::max_element(selected_genes.begin(), selected_genes.end(), [&](int a, int b) {
            return fitnesses[a] < fitnesses[b];
        });
        return genes[*max_it];
    };
}

/*
    Roulette Selection: Selection from all individuals using a roulette simulation, where higher fitness translates to higher probability
*/

std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&, std::mt19937&)> select_roulette() {
    return [](const std::vector<double>& fitnesses, const std::vector<std::vector<int>>& genes, std::mt19937& generator) -> std::vector<int> {
        double total_fitness = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
        std::vector<double> probabilities(fitnesses.size());
        std::transform(fitnesses.begin(), fitnesses.end(), probabilities.begin(), [&](double fitness) {
            return fitness / total_fitness;
        });
        std::partial_sum(probabilities.begin(), probabilities.end(), probabilities.begin());
        std::uniform_real_distribution< double > distribute_rate(0, 1);
        auto it = std::upper_bound(probabilities.begin(), probabilities.end(), distribute_rate(generator));
        int index = std::distance(probabilities.begin(), it);
        return genes[index];
    };
}

// Recombination Operators ----------------------------------------------------------

/*
    Midpoint Recombination: Takes the first half of the gene of the first parents gene and the second half of the second parents gene to create a new gene
*/

std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937&)> recombine_midpoint() { 
    return [](const std::vector<int>& parent1, const std::vector<int>& parent2, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> child(parent1.size());
        int midpoint = parent1.size() / 2;
        std::copy(parent1.begin(), parent1.begin() + midpoint, child.begin());
        std::copy(parent2.begin() + midpoint, parent2.end(), child.begin() + midpoint);
        return child;
    };
};

/*
    Twopoint Recombination: Takes a segment of the first parent and adds it to the childs gene. Then it fills out the residual chromosomes in the order of the second parent.
*/

std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937&)> recombine_twopoint() { 
    return [](const std::vector<int>& parent1, const std::vector<int>& parent2, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> child;
        std::uniform_real_distribution< double > distribute_point1(0, parent1.size() - 2);
        int i1 = distribute_point1(generator);
        std::uniform_real_distribution< double > distribute_point2(0, parent1.size() - i1 - 2);
        int i2 = distribute_point2(generator) + i1;
        std::vector<int> segment(parent1.begin() + i1, parent1.begin() + i2);
        int j = 0;
        while (child.size() < i1) {
            if (std::find(segment.begin(), segment.end(), parent2[j]) == segment.end() && std::find(child.begin(), child.end(), parent2[j]) == child.end()) {
                child.push_back(parent2[j]);
            }
            j++;
        }
        child.insert(child.end(), segment.begin(), segment.end());
        while (child.size() < parent1.size()) {
            if (std::find(child.begin(), child.end(), parent2[j]) == child.end()) {
                child.push_back(parent2[j]);
            }
            j++;
        }
        return child;
    };
};

// Evaluation Operators -------------------------------------------------------------

/*
    Sum Evaluation: Assigns fitness values proportional to the sum of the genes chromosomes
*/

std::function<double(const std::vector<int>&)> evaluate_sum() { 
    return [](const std::vector<int>& gene) -> double {
        int sum = 0;
        for(auto it = gene.begin(); it != gene.end(); ++it){
            sum += *it;
        }
        return sum;
    };
};

/*
    TSP Evaluation: Assigns fitness according to the reciprocated tsp objective function
*/

std::function<double(const std::vector<int>&)> evaluate_tsp(std::function<double(int x, int y)> distance_function) { 
    return [distance_function](const std::vector<int>& gene) -> double {
        int sum = 0;
        for(auto it = gene.begin(); it != std::prev(gene.end()); ++it){
            sum += distance_function(*it, *std::next(it));
        }
        sum += distance_function(gene.back(), gene.front());
        return 1/(double)sum;
    };
};

// Offspring selection operators ----------------------------------------------------

/*
    muh-Selection: Selects the best mu individuals from the combined population of parents and offspring
    
*/

template<typename T>
std::function<std::vector<T>(const std::vector<T>&, const std::vector<T>&)> select_muh(int mu, std::function<double(const T&)> evaluate) {
    return [mu](const std::vector<T>& parents, const std::vector<T>& offspring) -> std::vector<T> {
        std::vector<T> combined = parents;
        combined.insert(combined.end(), offspring.begin(), offspring.end());
        std::vector<T> selected_genes(mu);
        std::vector<double> fitnesses(combined.size());
        std::transform(combined.begin(), combined.end(), fitnesses.begin(), evaluate());
        std::vector<int> indices(combined.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::partial_sort(indices.begin(), indices.begin() + mu, indices.end(), [&](int a, int b) {
            return fitnesses[a] > fitnesses[b];
        });
        for (int i = 0; i < mu; i++) {
            selected_genes[i] = combined[indices[i]];
        }
        return selected_genes;
    };
};