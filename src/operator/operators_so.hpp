#include <functional>
#include <vector>
#include <cstdlib>
#include <numeric>

// Mutation Operators ---------------------------------------------------------------

/*  
    Numeric Mutation: Increment or decrement one bit by one
    Arguments:
        - mutation_rate:        overall probability of a mutation occurring
        - incrementation_rate:  probability of the mutation being an incrementation
        - chromosome_max:       maximum value for a chromosome
        - chromosome_min:       minimum value for a chromosome
*/

std::function<std::vector<int>(const std::vector<int>&)> mutate_numeric(double mutation_rate, double incrementation_rate, int chromosome_max, int chromosome_min) {
    return [mutation_rate, incrementation_rate, chromosome_max, chromosome_min](const std::vector<int>& gene) -> std::vector<int> {
        std::vector<int> mutated_gene = gene;
        double rand_num = (double)rand() / RAND_MAX;
        if (rand_num < mutation_rate) {
            int chromosome = rand() % gene.size();
            rand_num = (double)rand() / RAND_MAX;
            if (rand_num < incrementation_rate) {
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

std::function<std::vector<int>(const std::vector<int>&)> mutate_swap(double mutation_rate) {
    return [mutation_rate](const std::vector<int>& gene) -> std::vector<int> {
        std::vector<int> mutated_gene = gene;
        double rand_num = (double)rand() / RAND_MAX;
        if (rand_num < mutation_rate) {
            int chromosome1 = rand() % (gene.size()-1);
            int chromosome2 = rand() % (gene.size()-1);
            mutated_gene[chromosome1] = gene[chromosome2];
            mutated_gene[chromosome2] = gene[chromosome1];
        }
        return mutated_gene;
    };
}

/*
    No Mutation: No mutation is applied to the gene
*/

std::function<std::vector<int>(const std::vector<int>&)> mutate_no() {
    return [](const std::vector<int>& gene) -> std::vector<int> {
        return gene;
    };
}

// Parent Selection Operators -------------------------------------------------------

/*
    Tournament Parent Selection: Take a random subgroup of a specified size and choose the one with the highest fitness value
    Arguments:
        - tournament_size: size of the chosen subgroup
*/

std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&)> select_tournament(int tournament_size) {
    return [tournament_size](const std::vector<double>& fitnesses, const std::vector<std::vector<int>>& genes) -> std::vector<int> {
        std::vector<int> selected_genes(tournament_size);
        for (int i = 0; i < tournament_size; i++) {
            int rand_index = rand() % genes.size();
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

std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&)> select_roulette() {
    return [](const std::vector<double>& fitnesses, const std::vector<std::vector<int>>& genes) -> std::vector<int> {
        double total_fitness = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
        std::vector<double> probabilities(fitnesses.size());
        std::transform(fitnesses.begin(), fitnesses.end(), probabilities.begin(), [&](double fitness) {
            return fitness / total_fitness;
        });
        std::partial_sum(probabilities.begin(), probabilities.end(), probabilities.begin());
        double rand_num = (double)std::rand() / RAND_MAX;
        auto it = std::upper_bound(probabilities.begin(), probabilities.end(), rand_num);
        int index = std::distance(probabilities.begin(), it);
        return genes[index];
    };
}

/*
    No Parent Selection: First gene in population is chosen as parent    
*/

std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&)> select_no() {
    return [](const std::vector<double>& fitnesses, const std::vector<std::vector<int>>& genes) -> std::vector<int> {
        return genes[0];
    };
}

// Recombination Operators ----------------------------------------------------------

/*
    Midpoint Recombination: Takes the first half of the gene of the first parents gene and the second half of the second parents gene to create a new gene
*/

std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine_midpoint() { 
    return [](const std::vector<int>& parent1, const std::vector<int>& parent2) -> std::vector<int> {
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

std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine_twopoint() { 
    return [](const std::vector<int>& parent1, const std::vector<int>& parent2) -> std::vector<int> {
        std::vector<int> child;
        int i1 = std::rand() % (parent1.size() - 1);
        int i2 = std::rand() % (parent1.size() - i1 - 1) + i1;
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

/*
    No Recombination: Genes of first parent are returned
*/

std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine_no() { 
    return [](const std::vector<int>& parent1, const std::vector<int>& parent2) -> std::vector<int> {
        return parent1;
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

/*
    No Evaluation: Fitness is always 1
*/

std::function<double(const std::vector<int>&)> evaluate_no() { 
    return [](const std::vector<int>& gene) -> double {
        return 1;
    };
};