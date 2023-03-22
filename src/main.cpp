#include "population_so.hpp"
#include "operators_so.hpp"
#include "population_mo.hpp"
#include "operators_mo.hpp"

#include <iostream>
#include <vector>
#include <functional>

 int main(){

    std::function<double(const std::vector<int>&)> evaluate_so = evaluate_sum();
    std::function<std::vector<int>(const std::vector<int>&)> mutate_so = mutate_numeric(0.1, 0.5, 0, 9);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine_so = recombine_midpoint();
    std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&)> select_so = select_tournament(4);

    Population_SO population_so(100, 30, {0,1,2,3,4,5,6,7,8,9}, evaluate_so, mutate_so, recombine_so, select_so);
    population_so.execute_multiple(70, true, true);
    std::vector<int> best_so = population_so.getBest();
    for (auto chromosome : best_so) {
        std::cout << std::to_string(chromosome) + " ";
    }
    std::cout << " -> " << std::to_string(evaluate_so(best_so)) << "\n";
    
    std::function<std::vector<double>(const std::vector<int>&)> evaluate_mo = evaluate_bel3sum();
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&)> select_mo = select_tournament_rank(4);
    std::function<std::vector<int>(const std::vector<std::vector<double>>&)> rank_mo = rank_pareto();
    
    Population_MO population_mo(100, 30, {0,1,2,3,4,5,6,7,8,9}, evaluate_mo, mutate_so, recombine_so, rank_mo, select_mo);
    population_mo.execute_multiple(70, true, true);
    std::vector<std::vector<int>> bests_mo = population_mo.getBests();
    for (auto gene : bests_mo) {
        for (auto chromosome : gene) {
            std::cout << std::to_string(chromosome) + " ";
        }
        std::cout << " -> ";
        for (auto val : evaluate_mo(gene)) {
            std::cout << std::to_string(val) + " ";
        }
        std::cout << "\n";
    }    

    return 0;
}