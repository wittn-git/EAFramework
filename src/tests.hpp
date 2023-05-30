#include "population/population.hpp"
#include "operator/operators_parentSelection.hpp"
#include "operator/operators_mutation.hpp"
#include "operator/operators_recombination.hpp"
#include "operator/operators_survivorSelection.hpp"
#include "operator/operators_evaluation.hpp"
#include "operator/operators_initialization.hpp"
#include "operator/operators_ranking.hpp"

#include <iostream>

void test_scheduling(){

    std::vector<int> processing_times   = {1, 1, 2, 6, 10, 2,  7,  7,  9,  15};
    std::vector<int> release_dates      = {0, 0, 0, 1, 10, 30, 0,  2,  5,  1};
    std::vector<int> due_dates          = {10,2, 4, 7, 21, 36, 20, 10, 17, 17};

    using T = std::vector<int>;
    using L = std::vector<double>;

    std::function<std::vector<T>(std::mt19937&)> initialize_mo = initialize_random_permutation(100, {0,1,2,3,4,5,6,7,8,9});
    std::function<std::vector<L>(const std::vector<T>&)> evaluate_mo = evaluate_scheduling(processing_times, release_dates, due_dates);
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_parents_mo = select_tournament_rank<T, L>(4);
    std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> mutate_mo = mutate_extsigmablock(0.05, 3, due_dates);
    std::function<std::vector<int>(const std::vector<L>&)> rank_mo = rank_pareto<L>();
    
    Population<T, L> population_mo(0, initialize_mo, evaluate_mo, select_parents_mo, mutate_mo, nullptr, nullptr, rank_mo);
    population_mo.execute_multiple(100);
    std::vector<T> bests_mo = population_mo.get_bests(false);
    std::vector<L> bests_mo_eval = evaluate_mo(bests_mo);
    for (int i = 0; i < bests_mo.size(); i++) {
        for (auto chromosome : bests_mo[i]) {
            std::cout << std::to_string(chromosome) + " ";
        }
        std::cout << " -> ";
        for (auto val : bests_mo_eval[i]) {
            std::cout << std::to_string(1/val) + " ";
        }
        std::cout << "\n";
    }
}

void test_sum(){

    using T = std::vector<int>;
    using L = double;

    std::function<std::vector<T>(std::mt19937&)> initialize_so = initialize_random(100, 30, {0,1,2,3,4,5,6,7,8,9});
    std::function<std::vector<L>(const std::vector<T>&)> evaluate_so = evaluate_sum();
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_parents_so = select_tournament<T, L>(4);
    std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> mutate_so = mutate_numeric(0.1, 0.5, 0, 9);
    std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> recombine_so = recombine_midpoint();
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)> select_so = select_mu<T, L>(100, evaluate_so);

    Population<T, L> population_so(0, initialize_so, evaluate_so, select_parents_so, mutate_so, recombine_so, select_so, nullptr);
    population_so.execute_multiple(100);
    std::vector<T> best_so = population_so.get_bests(false);
    const std::vector<L> best_so_eval = evaluate_so(best_so);
    for (int i = 0; i < best_so.size(); i++) {
        for (auto chromosome : best_so[i]) {
            std::cout << std::to_string(chromosome) + " ";
        }
        std::cout << " -> " << std::to_string(best_so_eval[i]) << "\n";
    }
}

void test_bel3um(){

    using T = std::vector<int>;
    using L = std::vector<double>;
    

    std::function<std::vector<T>(std::mt19937&)> initialize_mo = initialize_random(100, 30, {0,1,2,3,4,5,6,7,8,9});
    std::function<std::vector<L>(const std::vector<T>&)> evaluate_mo = evaluate_bel3sum();
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_parents_mo = select_tournament_rank<T, L>(4);
    std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> mutate_mo = mutate_numeric(0.1, 0.5, 0, 9);
    std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> recombine_mo = recombine_midpoint();
    std::function<std::vector<int>(const std::vector<L>&)> rank_mo = rank_pareto<L>();
   
    Population<T, L> population_mo(0, initialize_mo, evaluate_mo, select_parents_mo, mutate_mo, recombine_mo, nullptr, rank_mo);
    population_mo.execute_multiple(70);
    std::vector<T> bests_mo = population_mo.get_bests(false);
    std::vector<L> bests_mo_eval = evaluate_mo(bests_mo);
    for (int i = 0; i < bests_mo.size(); i++) {
        for (auto chromosome : bests_mo[i]) {
            std::cout << std::to_string(chromosome) + " ";
        }
        std::cout << " -> ";
        for (auto val : bests_mo_eval[i]) {
            std::cout << std::to_string(val) + " ";
        }
        std::cout << "\n";
    }
}