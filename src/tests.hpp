#include "population/population_mo.hpp"
#include "population/population_so.hpp"
#include "operator/operators_mo.hpp"
#include "operator/operators_so.hpp"

#include <iostream>

void test_scheduling(){

    std::vector<int> processing_times   = {1, 1, 2, 6, 10, 2,  7,  7,  9,  15};
    std::vector<int> release_dates      = {0, 0, 0, 1, 10, 30, 0,  2,  5,  1};
    std::vector<int> due_dates          = {10,2, 4, 7, 21, 36, 20, 10, 17, 17};

    std::function<std::vector<int>(std::mt19937&)> initialize_mo = initialize_random_permutation({0,1,2,3,4,5,6,7,8,9});
    std::function<std::vector<double>(const std::vector<int>&)> evaluate_mo = evaluate_scheduling(processing_times, release_dates, due_dates);
    std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)> mutate_mo = mutate_extsigmablock(0.05, 3, due_dates);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937&)> recombine_mo = nullptr;
    std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&, std::mt19937&)> choose_mo = select_tournament_rank(4);
    std::function<std::vector<double>(const std::vector<std::vector<double>>&)> rank_mo = rank_pareto();
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&)> select_mo = select_offspring();

    Population_MO<std::vector<int>> population_mo(100, initialize_mo, evaluate_mo, mutate_mo, recombine_mo, rank_mo, choose_mo, select_mo, 0);
    population_mo.execute_multiple(100, false, true);
    std::set<std::vector<int>> bests_mo = population_mo.getBests();
    for (auto gene : bests_mo) {
        for (auto chromosome : gene) {
            std::cout << std::to_string(chromosome) + " ";
        }
        std::cout << " -> ";
        for (auto val : evaluate_mo(gene)) {
            std::cout << std::to_string(1/val) + " ";
        }
        std::cout << "\n";
    }
}

void test_sum(){

    std::function<std::vector<int>(std::mt19937&)> initialize_so = initialize_random(30, {0,1,2,3,4,5,6,7,8,9});
    std::function<double(const std::vector<int>&)> evaluate_so = evaluate_sum();
    std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)> mutate_so = mutate_numeric(0.1, 0.5, 0, 9);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937&)> recombine_so = recombine_midpoint();
    std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&, std::mt19937&)> choose_so = select_tournament(4);
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&)> select_so = select_muh(70, evaluate_so);

    Population_SO<std::vector<int>> population_so(100, initialize_so, evaluate_so, mutate_so, recombine_so, choose_so, select_so, 0);
    population_so.execute_multiple(70, true, true);
    std::vector<int> best_so = population_so.getBest();
    for (auto chromosome : best_so) {
        std::cout << std::to_string(chromosome) + " ";
    }
    std::cout << " -> " << std::to_string(evaluate_so(best_so)) << "\n";
}

void test_bel3um(){

    std::function<std::vector<int>(std::mt19937&)> initialize_mo = initialize_random(30, {0,1,2,3,4,5,6,7,8,9});
    std::function<std::vector<double>(const std::vector<int>&)> evaluate_mo = evaluate_bel3sum();
    std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)> mutate_mo = mutate_numeric(0.1, 0.5, 0, 9);
    std::function<std::vector<int>(const std::vector<int>&, const std::vector<int>&, std::mt19937&)> recombine_mo = recombine_midpoint();
    std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&, std::mt19937&)> choose_mo = select_tournament_rank(4);
    std::function<std::vector<double>(const std::vector<std::vector<double>>&)> rank_mo = rank_pareto();
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&)> select_mo = select_offspring();

    Population_MO<std::vector<int>> population_mo(100, initialize_mo, evaluate_mo, mutate_mo, recombine_mo, rank_mo, choose_mo, select_mo, 0);
    population_mo.execute_multiple(70, true, true);
    std::set<std::vector<int>> bests_mo = population_mo.getBests();
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

void test_noah(){

}