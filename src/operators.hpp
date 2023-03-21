#include <functional>
#include <vector>
#include <cstdlib>

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
            int chromosome2 = (chromosome1 + 1) + (rand() % (gene.size()-chromosome1));
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