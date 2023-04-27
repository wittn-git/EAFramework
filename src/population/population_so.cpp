#include "population_so.hpp"

Population_SO::Population_SO(
    const std::vector<std::vector<int>>& initial_genes, 
    const std::function<double(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&, std::mt19937& generator)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
    const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes, std::mt19937& generator)>& chooseParent,
    u32 seed
) : Population(initial_genes, seed), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {}

Population_SO::Population_SO(
    int size,
    int gene_length,
    const std::vector<int>& chromosome_list,
    const std::function<double(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&, std::mt19937& generator)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
    const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes, std::mt19937& generator)>& chooseParent,
    u32 seed
) : Population(size, gene_length, chromosome_list, seed), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {}

Population_SO::Population_SO(
    int size,
    const std::vector<int>& chromosome_list,
    const std::function<double(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&, std::mt19937& generator)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
    const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes, std::mt19937& generator)>& chooseParent,
    u32 seed
) : Population(size, chromosome_list, seed), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {}

void Population_SO::execute(bool useRecombination, bool useMutation) {
    std::vector<double> fitnessValues(size);
    for(int j = 0; j < size; j++){
        fitnessValues[j] = evaluate(genes[j]);
    }
    std::vector<std::vector<int>> new_genes = {};
    for(int j = 0; j < size; j++){
        std::vector<int> parent1 = chooseParent(fitnessValues, genes, generator);
        std::vector<int> child = parent1;
        if(useRecombination){
            std::vector<int> parent2 = chooseParent(fitnessValues, genes, generator);
            child = recombine(parent1, parent2, generator);
        }
        if(useMutation){
            child = mutate(child, generator);
        }
        new_genes.emplace_back(child);
    }
    genes = new_genes;
    
}

std::vector<int> Population_SO::getBest(){
    auto max_it = std::max_element(genes.begin(), genes.end(), [&](const auto& a, const auto& b) {
        return evaluate(a) < evaluate(b);
    });
    return *max_it;
}