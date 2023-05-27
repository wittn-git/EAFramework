#include <functional>
#include <vector>
#include <random>

// Parent Selection Operators -------------------------------------------------------

/*
    Tournament Parent Selection: Take a random subgroup of a specified size and choose the one with the best rank
    Arguments:
        - tournament_size: size of the chosen subgroup
        - rank:            function taking a vector of genes and returning a vector of ranks
*/

template<typename T>
std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> select_tournament_rank(int tournament_size, std::function<std::vector<int>(const std::vector<T>&)> rank) {
    return [tournament_size, rank](const std::vector<T>& genes, std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<int> ranks = rank(genes);
        std::vector<T> parents(genes.size());
        std::uniform_int_distribution< int > distribute_point(0, genes.size() - 1);
        std::transform(parents.begin(), parents.end(), parents.begin(), [&](T& parent) mutable -> T {
            T selected_genes(tournament_size);
            for (int i = 0; i < tournament_size; i++) {
                int rand_index = distribute_point(generator);
                selected_genes[i] = rand_index;
            }
            auto max_it = std::max_element(selected_genes.begin(), selected_genes.end(), [&](int a, int b) {
                return ranks[a] > ranks[b];
            });
            return genes[*max_it];
        });
        return parents;
    };
}

/*
    Roulette Selection: Selection from all individuals using a roulette simulation, where higher fitness translates to higher probability
    Arguments:
        - evaluate: function taking a vector of genes and returning a vector of fitnesses
*/

template <typename T, typename L>
std::function<std::vector<int>(const std::vector<std::vector<int>>&, std::mt19937&)> select_roulette(std::function<std::vector<L>(const std::vector<T>&)> evaluate) {
    return [evaluate](const std::vector<std::vector<int>>& genes, std::mt19937& generator) -> std::vector<int> {
        std::vector<L> fitnesses = evaluate(genes);
        std::vector<T> selected_genes(fitnesses.size());
        double total_fitness = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
        std::vector<double> probabilities(fitnesses.size());
        std::transform(fitnesses.begin(), fitnesses.end(), probabilities.begin(), [&](double fitness) {
            return fitness / total_fitness;
        });
        std::partial_sum(probabilities.begin(), probabilities.end(), probabilities.begin());
        std::uniform_real_distribution< double > distribute_rate(0, 1);
        std::transform(selected_genes.begin(), selected_genes.end(), selected_genes.begin(), [&](T& selected_gene) mutable -> T {
            auto it = std::upper_bound(probabilities.begin(), probabilities.end(), distribute_rate(generator));
            int index = std::distance(probabilities.begin(), it);
            return genes[index];
        });
        return selected_genes;
    };
}

/*
    Tournament Parent Selection: Take a random subgroup of a specified size and choose the one with the highest fitness value
    Arguments:
        - tournament_size: size of the chosen subgroup
        - evaluate:        function taking a vector of genes and returning a vector of fitnesses
*/

template<typename T, typename L>
std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> select_tournament(int tournament_size, std::function<std::vector<L>(const std::vector<T>&)> evaluate) {
    return [tournament_size, evaluate](const std::vector<std::vector<int>>& genes, std::mt19937& generator) -> std::vector<T> {
        std::vector<double> fitnesses = evaluate(genes);
        std::vector<T> selected_genes(genes.size());
        std::uniform_int_distribution< int > distribute_point(0, genes.size() - 1 );
        std::transform(selected_genes.begin(), selected_genes.end(), selected_genes.begin(), [&](T& selected_gene) mutable -> T {
            std::vector<int> tournament_genes(tournament_size);
            for (int i = 0; i < tournament_size; i++) {
                int rand_index = distribute_point(generator);
                tournament_genes[i] = rand_index;
            }
            auto max_it = std::max_element(tournament_genes.begin(), tournament_genes.end(), [&](int a, int b) {
                return fitnesses[a] < fitnesses[b];
            });
            return genes[*max_it];
        });
        return selected_genes;
    };
}