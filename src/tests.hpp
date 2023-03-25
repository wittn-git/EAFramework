#include "population_mo.hpp"
#include "population_so.hpp"
#include "operators_mo.hpp"
#include "operators_so.hpp"

#include <iostream>

void test_scheduling(){

    std::vector<int> processing_times = {1,1,2,6,10,2,7,7,9,15};
    std::vector<int> release_dates = {0,0,0,1,10,16,0,2,5,1};
    std::vector<int> due_dates = {10,2,4,7,21,18,20,10,17,17};
    
    std::function<std::vector<double>(const std::vector<int>&)> evaluate_mo = evaluate_scheduling(processing_times, release_dates, due_dates);
    std::function<std::vector<int>(const std::vector<int>&)> mutate_mo = mutate_sigmablock(0.05, 1, due_dates);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine_mo = nullptr;
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&)> select_mo = select_tournament_rank(4);
    std::function<std::vector<int>(const std::vector<std::vector<double>>&)> rank_mo = rank_pareto();

    Population_MO population_mo(100, {0,1,2,3,4,5,6,7,8,9}, evaluate_mo, mutate_mo, recombine_mo, rank_mo, select_mo);
    population_mo.execute_multiple(200, false, true);
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
}

void test_sum(){
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
}

void test_bel3um(){
    std::function<std::vector<double>(const std::vector<int>&)> evaluate_mo = evaluate_bel3sum();
    std::function<std::vector<int>(const std::vector<int>&)> mutate_mo = mutate_numeric(0.1, 0.5, 0, 9);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&)> recombine_mo = recombine_midpoint();
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&)> select_mo = select_tournament_rank(4);
    std::function<std::vector<int>(const std::vector<std::vector<double>>&)> rank_mo = rank_pareto();
    
    Population_MO population_mo(100, 30, {0,1,2,3,4,5,6,7,8,9}, evaluate_mo, mutate_mo, recombine_mo, rank_mo, select_mo);
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
}