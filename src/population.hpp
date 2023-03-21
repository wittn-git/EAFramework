#include <vector>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>

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
    std::vector<int> execute(); //executes one iteration of the evolutionary algorithm
    std::vector<std::vector<int>> execute(int generations); //executes 'generations' iterations of the evolutionary algorithm
    std::vector<std::vector<int>> get_genes();
    std::string to_string();
};