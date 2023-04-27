#include "population.hpp"

Population::Population(){};

Population::Population(const std::vector<std::vector<int>>& initial_genes, u32 seed) : size(initial_genes.size()), genes(initial_genes), generator(seed) {}

Population::Population(int size, int gene_length, const std::vector<int>& chromosome_list, u32 seed) : size(size), genes(size), generator(seed) {
    std::uniform_int_distribution< u32 > distribute( 0, chromosome_list.size()-1 );
    for (auto& gene : genes) {
        gene.resize(gene_length);
        for (int j = 0; j < gene_length; j++) {
            gene[j] = chromosome_list[distribute( generator )];
        }
    }
}

Population::Population(int size, const std::vector<int>& chromosome_list, u32 seed) : size(size), genes(size), generator(seed) {
    for (auto& gene : genes) {
        gene.resize(chromosome_list.size());
        gene = chromosome_list;
        std::shuffle(gene.begin(), gene.end(), generator);
    }    
}

std::vector<std::vector<int>> Population::get_genes(){
    return genes;
}

void Population::execute_multiple(int generations, bool useRecombination, bool useMutation){
    for(int i = 0; i < generations; i++){
        execute(useRecombination, useMutation);
    }
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