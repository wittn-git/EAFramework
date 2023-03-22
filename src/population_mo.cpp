#include "population_mo.hpp"

Population_MO::Population_MO(
    const std::vector<std::vector<int>>& initial_genes, 
    const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
    const std::function<std::vector<int>(const std::vector<std::vector<double>>&, const std::vector<std::vector<int>>& genes)>& chooseParent
) : Population(initial_genes), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {}

Population_MO::Population_MO(
    int size,
    int gene_length,
    const std::vector<int>& chromosome_list,
    const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
    const std::function<std::vector<int>(const std::vector<std::vector<double>>&, const std::vector<std::vector<int>>& genes)>& chooseParent
) : Population(size, gene_length, chromosome_list), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {}

Population_MO::Population_MO(
    int size,
    const std::vector<int>& chromosome_list,
    const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
    const std::function<std::vector<int>(const std::vector<std::vector<double>>&, const std::vector<std::vector<int>>& genes)>& chooseParent
) : Population(size, chromosome_list), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {}

void Population_MO::execute(bool useRecombination, bool useMutation) {
    std::vector<std::vector<double>> fitnessValues(size);
    for(int j = 0; j < size; j++){
        fitnessValues[j] = evaluate(genes[j]);
    }
    std::vector<std::vector<int>> new_genes = {};
    for(int j = 0; j < size; j++){
        std::vector<int> parent1 = chooseParent(fitnessValues, genes);
        std::vector<int> child = parent1;
        if(useRecombination){
            std::vector<int> parent2 = chooseParent(fitnessValues, genes);
            child = recombine(parent1, parent2);
        }
        if(useMutation){
            child = mutate(child);
        }
        new_genes.emplace_back(child);
    }
    genes = new_genes;
}