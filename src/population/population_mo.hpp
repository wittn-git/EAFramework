#include "population.hpp"

class Population_MO : public Population{

private:
    const std::function<std::vector<double>(const std::vector<int>&)>& evaluate;
    const std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)>& mutate;
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine;
    const std::function<std::vector<int>(const std::vector<std::vector<double>>&)>& rank;
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&, std::mt19937& generator)>& chooseParent;

public:

    // Constructor for population initialized with passed genes initial_genes
    Population_MO(
        const std::vector<std::vector<int>>& initial_genes, 
        const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
        const std::function<std::vector<int>(const std::vector<std::vector<double>>&)>& rank,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>& genes, std::mt19937& generator)>& chooseParent,
        u32 seed
    );

    // Constructor for population initialized with random genes of length gene_length using the chromosomes of chromosome_list
    Population_MO(
        int size,
        int gene_length,
        const std::vector<int>& chromosome_list,
        const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
        const std::function<std::vector<int>(const std::vector<std::vector<double>>&)>& rank,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&, std::mt19937& generator)>& chooseParent,
        u32 seed
    );

    // Constructor for population initialized with random permutations of the chromosome_list
    Population_MO(
        int size,
        const std::vector<int>& chromosome_list,
        const std::function<std::vector<double>(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937& generator)>& recombine,
        const std::function<std::vector<int>(const std::vector<std::vector<double>>&)>& rank,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&, std::mt19937& generator)>& chooseParent,
        u32 seed
    );

    void execute(bool useRecombination, bool useMutation); //executes one iteration of the evolutionary algorithm
    std::set<std::vector<int>> getBests(); //returns the genes with the highest rank in the current population
};