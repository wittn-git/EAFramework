#include "population.hpp"

#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <algorithm> 

int main(){
    std::function<double(const std::vector<int>&)> evaluate = [](const std::vector<int>& gene) {
        int sum = 0;
        for(auto it = gene.begin(); it != gene.end(); ++it){
            sum += *it;
        }
        return sum;
    };
    std::function<std::vector<int>(const std::vector<int>&)> mutate = [](const std::vector<int>& gene) {
        std::vector<int> mutated_gene = gene;
        double rand_num = (double)rand() / RAND_MAX;
        if (rand_num < 0.1) {
            int chromosome = rand() % gene.size();
            rand_num = (double)rand() / RAND_MAX;
            if (rand_num < 0.5) {
                mutated_gene[chromosome] = std::max(0, mutated_gene[chromosome] - 1);
            }
            else {
                mutated_gene[chromosome] = std::min(9, mutated_gene[chromosome] + 1);
            }
        }
        return mutated_gene;
    };
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine = [](const std::vector<int>& parent1, const std::vector<int>& parent2) {
        std::vector<int> child(parent1.size());
        int midpoint = parent1.size() / 2;
        std::copy(parent1.begin(), parent1.begin() + midpoint, child.begin());
        std::copy(parent2.begin() + midpoint, parent2.end(), child.begin() + midpoint);
        return child;
    };
    std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)> chooseParent = [](const std::vector<double>& fitnesses, const std::vector<std::vector<int>>& genes) {
        int tournament_size = 5;
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

    Population population(20, 30, {0,1,2,3,4,5,6,7,8,9}, evaluate, mutate, recombine, chooseParent);
    std::vector<std::vector<int>> bests = population.execute(200);
    for(auto gene : bests) {
        for (auto chromosome : gene) {
            std::cout << std::to_string(chromosome) + " ";
        }
        std::cout << " -> " << std::to_string(evaluate(gene)) << "\n";
    }
    return 0;
}