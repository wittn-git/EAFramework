#include "population.hpp"

Population::Population(){};

Population::Population(const std::vector<std::vector<int>>& initial_genes) : size(initial_genes.size()), genes(initial_genes) {
    std::srand(std::time(nullptr));
}

Population::Population(int size, int gene_length, const std::vector<int>& chromosome_list) : size(size), genes(size) {
    std::srand(std::time(nullptr));
    for (auto& gene : genes) {
        gene.resize(gene_length);
        for (int j = 0; j < gene_length; j++) {
            gene[j] = chromosome_list[rand() % chromosome_list.size()];
        }
    }
}

Population::Population(int size, const std::vector<int>& chromosome_list) : size(size), genes(size) {
    std::srand(std::time(nullptr));
    std::random_device rd;
    std::mt19937 g(rd());
    for (auto& gene : genes) {
        gene.resize(chromosome_list.size());
        gene = chromosome_list;
        std::shuffle(gene.begin(), gene.end(), g);
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