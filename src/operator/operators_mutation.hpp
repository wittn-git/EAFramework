#include <functional>
#include <vector>
#include <random>
#include <cstdint>

using u32 = uint_least32_t;

// Mutation Operators ---------------------------------------------------------------

/*  
    Numeric Mutation: Increment or decrement one bit by one
    Arguments:
        - mutation_rate:        overall probability of a mutation occurring
        - incrementation_rate:  probability of the mutation being an incrementation
        - chromosome_max:       maximum value for a chromosome
        - chromosome_min:       minimum value for a chromosome
*/

std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_numeric(double mutation_rate, double incrementation_rate, int chromosome_max, int chromosome_min) {
    return [mutation_rate, incrementation_rate, chromosome_max, chromosome_min](const std::vector<std::vector<int>>& genes, std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> mutated_genes(genes.size());
        std::transform(genes.begin(), genes.end(), mutated_genes.begin(), [mutation_rate, incrementation_rate, chromosome_max, chromosome_min, generator](const std::vector<int>& gene) -> std::vector<int> {
            std::vector<int> mutated_gene = gene;
            std::uniform_real_distribution< double > distribute_rate(0, 1);
            if (distribute_rate(generator) < mutation_rate) {
                std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 1 );
                int chromosome = distribute_point(generator);
                if (distribute_rate(generator) < incrementation_rate) {
                    mutated_gene[chromosome] = std::min(chromosome_max, mutated_gene[chromosome] + 1);
                } else {
                    mutated_gene[chromosome] = std::max(chromosome_min, mutated_gene[chromosome] - 1);
                }
            }
            return mutated_gene;
        });
        return mutated_genes;
    };
}

/*  
    Swap Mutation: Swap two bits
    Arguments:
        - mutation_rate:        overall probability of a mutation occurring
*/

std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_swap(double mutation_rate) {
    return [mutation_rate](const std::vector<std::vector<int>>& genes, std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> mutated_genes(genes.size());
        std::transform(genes.begin(), genes.end(), mutated_genes.begin(), [mutation_rate, generator](const std::vector<int>& gene) -> std::vector<int> {
            std::vector<int> mutated_gene = gene;
            std::uniform_real_distribution< double > distribute_rate(0, 1);
            if (distribute_rate(generator) < mutation_rate) {
                std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 1 );
                int chromosome1 =  distribute_point(generator);
                int chromosome2 =  distribute_point(generator);
                mutated_gene[chromosome1] = gene[chromosome2];
                mutated_gene[chromosome2] = gene[chromosome1];
            }
            return mutated_gene;
        });
        return mutated_genes;
    };
}

/*
    Sigma Block Mutation: Take a random point and the block of 2 times sigma around it, then sort it according to the EDD rule (earliest due date first)
    Arguments:
        - mutation_rate:    probability of mutation
        - sigma:            size of the block
        - due_dates:        Points in time where every job is due
*/

std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_sigmablock(double mutation_rate, int sigma, std::vector<int> due_dates) {
    return [mutation_rate, sigma, due_dates](const std::vector<std::vector<int>>& gene, std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> mutated_genes(gene.size());
        std::transform(gene.begin(), gene.end(), mutated_genes.begin(), [mutation_rate, sigma, due_dates, generator](const std::vector<int>& gene) -> std::vector<int> {
            std::vector<int> mutated_gene = gene;
            std::uniform_real_distribution< double > distribute_rate(0, 1);
            std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 2*sigma - 1 );
            if (distribute_rate(generator) < mutation_rate) {
                int midpoint = sigma + distribute_point(generator);
                std::vector<int> block(mutated_gene.begin() + midpoint - sigma, mutated_gene.begin() + midpoint + sigma + 1);
                std::sort(block.begin(), block.end(), [due_dates](int a, int b) {
                    return due_dates[a] < due_dates[b];
                });
                std::copy(block.begin(), block.end(), mutated_gene.begin() + midpoint - sigma);
            }
            return mutated_gene;
        });
        return mutated_genes;
    };
}

/*
    Extended Sigma Block Mutation: Take sigma random positions, then sort it according to the EDD rule (earliest due date first)
    Arguments:
        - mutation_rate:    probability of mutation
        - sigma:            size of the block
        - due_dates:        Points in time where every job is due

*/

std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_extsigmablock(double mutation_rate, int sigma, std::vector<int> due_dates) {
    return [mutation_rate, sigma, due_dates](const std::vector<std::vector<int>>& gene, std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> mutated_genes(gene.size());
        std::transform(gene.begin(), gene.end(), mutated_genes.begin(), [mutation_rate, sigma, due_dates, generator](const std::vector<int>& gene) -> std::vector<int> {
            std::vector<int> mutated_gene = gene;
            std::uniform_real_distribution< double > distribute_rate(0, 1);
            std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 1 );
            if (distribute_rate(generator) < mutation_rate) {
                std::vector<int> indices, points;
                for(int i = 0; i < sigma; i++){
                    int index = distribute_point(generator);
                    indices.emplace_back(index);
                    points.emplace_back(gene[index]);
                }
                std::sort(points.begin(), points.end(), [due_dates](int a, int b) {
                    return due_dates[a] < due_dates[b];
                });
                for(int i = 0; i < sigma; i++){
                    mutated_gene[indices[i]] = points[i];
                }
            }
            return mutated_gene;
        });
        return mutated_genes;
    };
}