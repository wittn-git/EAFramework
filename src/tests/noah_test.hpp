#include <vector>

#include "../population/population.hpp"
#include "../operator/operators_initialization.hpp"
#include "../operator/operators_evaluation.hpp"
#include "../operator/operators_parentSelection.hpp"
#include "../operator/operators_mutation.hpp"

std::vector<std::vector<int>> NOAH(int population_size, double barrier_value, int generations_objopt, int remaining_solutions, int generations_divopt){
    using T = std::vector<int>;
    using L = double;

    std::function<std::vector<T>(std::mt19937&)> initialize = initialize_random(100, 30, {0,1,2,3,4,5,6,7,8,9});
    std::function<std::vector<L>(const std::vector<T>&)> evaluate = evaluate_sum();
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_parents = select_tournament<T, L>(4);
    std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> mutate = mutate_numeric(0.1, 0.5, 0, 9);


}