#pragma once

#include <vector>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <random>

class Population{

protected:
    int size;
    std::vector<std::vector<int>> genes;

public:

    Population();

    // Constructor for population initialized with passed genes initial_genes
    Population(
        const std::vector<std::vector<int>>& initial_genes
    );

    // Constructor for population initialized with random genes of length gene_length using the chromosomes of chromosome_list
    Population(
        int size,
        int gene_length,
        const std::vector<int>& chromosome_list
    );
    
    // Constructor for population initialized with random permutations of the chromosome_list
    Population(
        int size,
        const std::vector<int>& chromosome_list
    );

    virtual void execute(bool useRecombination, bool useMutation) = 0; //executes one iteration of the evolutionary algorithm
    void execute_multiple(int generations, bool useRecombination, bool useMutation); //executes 'generations' iterations of the evolutionary algorithm
    std::vector<std::vector<int>> get_genes();
    std::string to_string();
};