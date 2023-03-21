#include "population.hpp"
#include "operators.hpp"

#include <iostream>
#include <vector>
#include <functional>

int main(){
    std::function<double(const std::vector<int>&)> evaluate = [](const std::vector<int>& gene) {
        int sum = 0;
        for(auto it = gene.begin(); it != gene.end(); ++it){
            sum += *it;
        }
        return sum;
    };
    std::function<std::vector<int>(const std::vector<int>&)> mutate = mutate_numeric(1, 0.5, 9, 0);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine = recombine_midpoint();
    std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&)> select = select_tournament(5);

    Population population(50, 30, {0,1,2,3,4,5,6,7,8,9}, evaluate, mutate, recombine, select);
    std::vector<std::vector<int>> bests = population.execute(10);
    for(auto gene : bests) {
        for (auto chromosome : gene) {
            std::cout << std::to_string(chromosome) + " ";
        }
        std::cout << " -> " << std::to_string(evaluate(gene)) << "\n";
    }
    return 0;
}