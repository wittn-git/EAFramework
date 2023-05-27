#include <functional>
#include <vector>
#include <random>

//Initialization Operators ----------------------------------------------------------

/*
    Random Initialization: Randomly initialize a gene with chromosomes from chromosome_list
    Arguments:
        - population_size:      number of genes in population
        - gene_length:          number of chromosomes in gene
        - chromosome_list:      list of possible values for each chromosome
*/

std::function<std::vector<std::vector<int>>(std::mt19937&)> initialize_random(int population_size, int gene_length, const std::vector<int> chromosome_list) {
    return [population_size, gene_length, chromosome_list](std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> genes(population_size);
        std::uniform_int_distribution< int > distribute_chromosome(0, chromosome_list.size() - 1);
        std::transform(genes.begin(), genes.end(), genes.begin(), [gene_length, chromosome_list, &generator, distribute_chromosome](std::vector<int>& gene) mutable -> std::vector<int> {
            for (int i = 0; i < gene_length; i++) {
                gene.emplace_back(chromosome_list[distribute_chromosome(generator)]);
            }
            return gene;
        });
        return genes;
    };
}

/*
    Random Permutation Initialization: Randomly initialize a gene with a permutation of chromosome_list
    Arguments:
        - population_size:      number of genes in population
        - chromosome_list:      elements of permutation
*/

std::function<std::vector<std::vector<int>>(std::mt19937&)> initialize_random_permutation(int population_size, const std::vector<int> chromosome_list) {
    return [population_size, chromosome_list](std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> genes(population_size);
        std::transform(genes.begin(), genes.end(), genes.begin(), [chromosome_list, &generator](std::vector<int>& gene) -> std::vector<int> {
            gene = chromosome_list;
            std::shuffle(gene.begin(), gene.end(), generator);
            return gene;
        });
        return genes;
    };
}