#include <vector>
#include <functional>
#include <random>
#include <algorithm>

class Population{

private:
    int size;
    std::vector<std::vector<int>> genes;
    const std::function<double(const std::vector<int>&)>& evaluate;
    const std::function<std::vector<int>(const std::vector<int>&)>& mutate;
    const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine;
    const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent;

public:
    Population(
        const std::vector<std::vector<int>>& initial_genes, 
        const std::function<double(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
        const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent
    );
    Population(
        int size,
        int gene_length,
        const std::vector<int>& chromosome_list,
        const std::function<double(const std::vector<int>&)>& evaluate,
        const std::function<std::vector<int>(const std::vector<int>&)>& mutate,
        const std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)>& recombine,
        const std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>& genes)>& chooseParent
    );
    std::vector<int> execute();
    std::vector<std::vector<int>> execute(int generations);
    std::string to_string();
};