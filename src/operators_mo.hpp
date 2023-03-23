#include <functional>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <utility>
#include <ranges>
#include <set>

// Parent Selection Operators -------------------------------------------------------

/*
    Tournament Parent Selection: Take a random subgroup of a specified size and choose the one with the best rank
    Arguments:
        - tournament_size: size of the chosen subgroup
*/

std::function<std::vector<int>(const std::vector<int>&, const std::vector<std::vector<int>>&)> select_tournament_rank(int tournament_size) {
    return [tournament_size](const std::vector<int>& ranks, const std::vector<std::vector<int>>& genes) -> std::vector<int> {
        std::vector<int> selected_genes(tournament_size);
        for (int i = 0; i < tournament_size; i++) {
            int rand_index = rand() % genes.size();
            selected_genes[i] = rand_index;
        }
        auto max_it = std::max_element(selected_genes.begin(), selected_genes.end(), [&](int a, int b) {
            return ranks[a] > ranks[b];
        });
        return genes[*max_it];
    };
}

// Evaluation Operators -------------------------------------------------------------

/*
    Below3 Sum Evaluation: Assigns one fitness values proportional to the sum of the genes chromosomes and one based on the occurence of numbers below 3
*/

std::function<std::vector<double>(const std::vector<int>&)> evaluate_bel3sum() { 
    return [](const std::vector<int>& gene) -> std::vector<double> {
        int sum = 0;
        for(auto it = gene.begin(); it != gene.end(); ++it){
            sum += *it;
        }
        int below3_n = 0;
        for(auto it = gene.begin(); it != gene.end(); ++it){
            if(*it < 3) below3_n++;
        }
        return {(double) sum, (double) below3_n};
    };
}

// Ranking Operators-----------------------------------------------------------------

/*
    Pareto Ranking - Ranks vectors of fitness values based on pareto optimality
*/
std::function<std::vector<int>(const std::vector<std::vector<double>>&)> rank_pareto() { 
    return [](const std::vector<std::vector<double>>& fitnesses) -> std::vector<int> {
        std::vector<int> ranks(fitnesses.size(), 0);
        int current_rank = 1;
        int items_ranked = 0;
        while(items_ranked < fitnesses.size()){
            std::set<int> currently_ranked = {};
            for(int i = 0; i < fitnesses.size(); i++){
                if(ranks[i] != 0) continue;
                for(int j = 0; j < fitnesses.size(); j++){
                    if(i == j || ranks[j] != 0) continue;
                    bool strict = false;
                    for(int k = 0; k < fitnesses[i].size(); k++){
                        if(fitnesses[i][k] > fitnesses[j][k]){
                            goto not_dominated;
                        }
                        if(fitnesses[i][k] < fitnesses[j][k]){
                            strict = true;
                        }
                    }
                    if(strict) goto dominated;
                    not_dominated:;
                }
                currently_ranked.emplace(i);
                dominated:;
            }
            for(auto it = currently_ranked.begin(); it != currently_ranked.end(); ++it){
                ranks[*it] = current_rank;
            }
            items_ranked += currently_ranked.size();
            current_rank++;
        }
        return ranks;
    };
}