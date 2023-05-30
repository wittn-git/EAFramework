#include <vector>
#include <limits>
#include <tuple>

#include "../population/population.hpp"
#include "../operator/operators_initialization.hpp"
#include "../operator/operators_evaluation.hpp"
#include "../operator/operators_parentSelection.hpp"
#include "../operator/operators_mutation.hpp"

using T = std::vector<int>;
using L = double;


std::function<std::vector<T>(std::vector<T>&, std::vector<L>&, std::vector<int>&, std::vector<T>&, std::mt19937&)> survivor_selection_ObjOpt(double b, int max_size, std::function<std::vector<L>(const std::vector<T>&)> evaluate){
    return [b, max_size, evaluate](const std::vector<T>& parents, const std::vector<L>& fitnesses_parents, const std::vector<int>& ranks_parents, const std::vector<T>& offspring, std::mt19937& generator) -> std::vector<T> {
        std::vector<T> combined = parents;
        combined.insert(combined.end(), offspring.begin(), offspring.end());
        std::vector<L> fitnesses = fitnesses_parents;
        std::vector<L> fitnesses_children = evaluate(offspring);
        fitnesses.insert(fitnesses.end(), fitnesses_children.begin(), fitnesses_children.end());
        std::vector<T> selected_genes;
        std::vector<int> indices(combined.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&](int a, int b) {
            return fitnesses[a] > fitnesses[b];
        });
        for (int i = 0; i < max_size; i++) {
            selected_genes.emplace_back(combined[indices[i]]);
            if(fitnesses[indices[i]] < b){
                break;
            }
        }
        return selected_genes;
    }; 
}

std::function<std::vector<T>(std::mt19937&)> initialize = initialize_random(100, 30, {0,1,2,3,4,5,6,7,8,9});
std::function<std::vector<L>(const std::vector<T>&)> evaluate = evaluate_sum();
std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_parents = select_tournament<T, L>(4);
std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> mutate = mutate_numeric(0.1, 0.5, 0, 9);

Population<T, L> objective_optimization(Population<T, L> population, int generations, int bound){
    return population;
}

Population<T, L> diversity_optimization(Population<T, L> population, int population_size, int bound_value, int generations_divopt){
    // Do divopt until div does not improve for divopt generations
    return population;
}

std::tuple<Population<T, L>, double> bound_change(Population<T, L> population, int bound, int r){
    // set bound on value that r solution in population have, return new pop and new bound
    return std::make_tuple(population, 0.0);
}

std::vector<T> NOAH(int population_size, double barrier_value, int generations_objopt, int remaining_solutions, int generations_divopt){
    
    Population<T, L> population(0, initialize, evaluate, select_parents, mutate, nullptr, nullptr, nullptr);
    double bound_value = std::numeric_limits<double>::infinity();
    int overall_generations = 0;
    while(bound_value > barrier_value && overall_generations < 500){
        overall_generations++;
        population = objective_optimization(population, generations_objopt, bound_value);
        std::tie(population, bound_value) = bound_change(population, bound_value, remaining_solutions);
        population = diversity_optimization(population, population_size, bound_value, generations_divopt);
    }
    return population.get_genes();
}