#include "population.hpp"

Population::Population(
    const std::vector<std::vector<int>>& initial_genes, 
    const std::function<double(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
    const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent
) : size(initial_genes.size()), genes(initial_genes), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {}

Population::Population(
    int size,
    int gene_length,
    const std::vector<int>& chromosome_list,
    const std::function<double(const std::vector<int>&)>& evaluate,
    const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
    const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent
) : size(size), genes(size), evaluate(evaluate), mutate(mutate), recombine(recombine), chooseParent(chooseParent) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, chromosome_list.size()-1);
    for (auto& gene : genes) {
        gene.resize(gene_length);
        for (int j = 0; j < gene_length; j++) {
            gene[j] = chromosome_list[dist(gen)];
        }
    }
}

std::vector<int> Population::execute(){
    std::vector<double> fitnessValues(size);
    for(int j = 0; j < size; j++){
        fitnessValues[j] = evaluate(genes[j]);
    }
    std::vector<std::vector<int>> new_genes = {};
    for(int j = 0; j < size; j++){
        std::vector<int> parent1 = chooseParent(fitnessValues, genes);
        std::vector<int> parent2 = chooseParent(fitnessValues, genes);
        std::vector<int> child = recombine(parent1, parent2);
        std::vector<int> mutated_child = mutate(child);
        new_genes.emplace_back(mutated_child);
    }
    genes = new_genes;
    auto max_it = std::max_element(genes.begin(), genes.end(), [&](const auto& a, const auto& b) {
        return evaluate(a) < evaluate(b);
    });
    return *max_it;
}

std::vector<std::vector<int>> Population::execute(int generations){
    std::vector<std::vector<int>> bests;
    for(int i = 0; i < generations; i++){
        bests.emplace_back(execute());
    }
    return bests;
}

std::string Population::to_string(){
    std::string s;
    for (auto gene : genes) {
        for (auto chromosome : gene) {
            s += std::to_string(chromosome) + " ";
        }
        s += "\n";
    }
    return s;
}