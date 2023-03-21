#include <vector>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <random>

class Population{

private:
    int size;
    std::vector<std::vector<int>> genes;
    const std::function<double(const std::vector<int>&)>& evaluate;
    const std::function<std::vector<int>(const std::vector<int>&)>& mutate;
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine;
    const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent;

public:
    // Constructor for population initialized with passed genes initial_genes
    Population(
        const std::vector<std::vector<int>>& initial_genes, 
        const std::function<double(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
        const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent
    );
    // Constructor for population initialized with random genes of length gene_length using the chromosomes of chromosome_list
    Population(
        int size,
        int gene_length,
        const std::vector<int>& chromosome_list,
        const std::function<double(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
        const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent
    );
    // Constructor for population initialized with random permutations of the chromosome_list
    Population(
        int size,
        const std::vector<int>& chromosome_list,
        const std::function<double(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
        const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent
    );
    std::vector<int> execute(); //executes one iteration of the evolutionary algorithm
    std::vector<std::vector<int>> execute(int generations); //executes 'generations' iterations of the evolutionary algorithm
    std::vector<std::vector<int>> get_genes();
    std::string to_string();
};