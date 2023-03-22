#include "population_so.hpp"
#include "operators_so.hpp"

#include <iostream>
#include <vector>
#include <functional>

 int main(){

    std::function<double(const std::vector<int>&)> evaluate = evaluate_sum();
    std::function<std::vector<int>(const std::vector<int>&)> mutate = mutate_numeric(0.1, 0.5, 0, 9);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine = recombine_midpoint();
    std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&)> select = select_tournament(4);

    Population_SO population(100, 30, {0,1,2,3,4,5,6,7,8,9}, evaluate, mutate, recombine, select);
    population.execute_multiple(70, true, true);
    std::vector<int> best = population.getBest();
    for (auto chromosome : best) {
        std::cout << std::to_string(chromosome) + " ";
    }
    std::cout << " -> " << std::to_string(evaluate(best)) << "\n";
    

    return 0;
}