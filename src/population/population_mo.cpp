#include "population_mo.hpp"

Population_MO::Population_MO(
    const std::vector<std::vector<int>>& initial_genes, 
    const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
    const std::function<std::vector<int>(const std::vector<std::vector<double>>&)>& rank,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&, std::mt19937& generator)>& chooseParent,
    u32 seed
) : Population(initial_genes, seed), evaluate(evaluate), mutate(mutate), recombine(recombine), rank(rank), chooseParent(chooseParent) {}

Population_MO::Population_MO(
    int size,
    int gene_length,
    const std::vector<int>& chromosome_list,
    const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
    const std::function<std::vector<int>(const std::vector<std::vector<double>>&)>& rank,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&, std::mt19937& generator)>& chooseParent,
    u32 seed
) : Population(size, gene_length, chromosome_list, seed), evaluate(evaluate), mutate(mutate), recombine(recombine), rank(rank), chooseParent(chooseParent) {}

Population_MO::Population_MO(
    int size,
    const std::vector<int>& chromosome_list,
    const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
    const std::function<std::vector<int>(const std::vector<std::vector<double>>&)>& rank,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>& genes, std::mt19937& generator)>& chooseParent,
    u32 seed
) : Population(size, chromosome_list, seed), evaluate(evaluate), mutate(mutate), recombine(recombine), rank(rank), chooseParent(chooseParent) {}

void Population_MO::execute(bool useRecombination, bool useMutation) {
    std::vector<std::vector<double>> fitnessValues(size);
    for(int j = 0; j < size; j++){
        fitnessValues[j] = evaluate(genes[j]);
    }
    std::vector<std::vector<int>> new_genes = {};
    std::vector<int> ranks = rank(fitnessValues);
    for(int j = 0; j < size; j++){        
        std::vector<int> parent1 = chooseParent(ranks, genes, generator);
        std::vector<int> child = parent1;
        if(useRecombination){
            std::vector<int> parent2 = chooseParent(ranks, genes, generator);
            child = recombine(parent1, parent2, generator);
        }
        if(useMutation){
            child = mutate(child, generator);
        }
        new_genes.emplace_back(child);
    }
    genes = new_genes;
}

std::set<std::vector<int>> Population_MO::getBests(){
    std::vector<std::vector<double>> fitnessValues(size);
    for(int i = 0; i < size; i++){
        fitnessValues[i] = evaluate(genes[i]);
    }
    std::vector<int> ranks = rank(fitnessValues);
    std::set<std::vector<int>> bests;
    for(int i = 0; i < size; i++){
        if(ranks[i] == 1) bests.emplace(genes[i]);
    }
    return bests;
}