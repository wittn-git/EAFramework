#include <functional>
#include <vector>
#include <cstdlib>
#include <numeric>

using u32 = uint_least32_t; 

// Survivor selection operators ----------------------------------------------------

/*
    muh-Selection: Selects the best mu individuals from the combined population of parents and offspring
    Arguments:
        - mu:       number of individuals to select
        - evaluate: function taking a vector of genes and returning a vector of fitnesses
*/

template<typename T>
std::function<std::vector<T>(const std::vector<T>&, const std::vector<T>&)> select_muh(int mu, std::function<std::vector<double>(const T&)> evaluate) {
    return [mu, evaluate](const std::vector<T>& parents, const std::vector<T>& offspring) -> std::vector<T> {
        std::vector<T> combined = parents;
        combined.insert(combined.end(), offspring.begin(), offspring.end());
        std::vector<T> selected_genes(mu);
        std::vector<double> fitnesses = evaluate(combined);
        std::vector<int> indices(combined.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::partial_sort(indices.begin(), indices.begin() + mu, indices.end(), [&](int a, int b) {
            return fitnesses[a] > fitnesses[b];
        });
        for (int i = 0; i < mu; i++) {
            selected_genes[i] = combined[indices[i]];
        }
        return selected_genes;
    };
};