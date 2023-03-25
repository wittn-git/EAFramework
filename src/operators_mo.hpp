#include <functional>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <utility>
#include <ranges>
#include <set>

// Mutation Operators ---------------------------------------------------------------

/*
    Sigma Block Mutation: Take a random point and the block of 2 times sigma around it, then sort it according to the EDD rule (earliest due date first)
*/

std::function<std::vector<int>(const std::vector<int>&)> mutate_sigmablock(double mutation_rate, int sigma, std::vector<int> due_dates) {
    return [mutation_rate, sigma, due_dates](const std::vector<int>& gene) -> std::vector<int> {
        std::vector<int> mutated_gene = gene;
        double rand_num = (double)rand() / RAND_MAX;
        if (rand_num < mutation_rate) {
            int midpoint = sigma + rand() % (gene.size() - 2*sigma);
            std::vector<int> block(mutated_gene.begin() + midpoint - sigma, mutated_gene.begin() + midpoint + sigma + 1);
            std::sort(block.begin(), block.end(), [due_dates](int a, int b) {
                return due_dates[a] < due_dates[b];
            });
            std::copy(block.begin(), block.end(), mutated_gene.begin() + midpoint - sigma);
        }
        return mutated_gene;
    };
}

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

/*
    Scheduling Evaluation: Evaluates Machine Jobs based on maximum lateness, number of tardy jobs and overall completion time
    Arguments:
        - processing_times: Times it takes to complete each job
        - release_dates:    Points in time where every job is released
        - due_dates:        Points in time where every job is due
*/

std::function<std::vector<double>(const std::vector<int>&)> evaluate_scheduling(std::vector<int> processing_times, std::vector<int> release_dates, std::vector<int> due_dates) { 
    return [processing_times, release_dates, due_dates](const std::vector<int>& gene) -> std::vector<double> {
        std::vector<int> start_points;
        int current_time = 0;
        for(auto it = gene.begin(); it != gene.end(); it++){
            if(release_dates[*it] > current_time){
                start_points.emplace_back(release_dates[*it]);
                current_time = release_dates[*it] + processing_times[*it];
            }else{
                start_points.emplace_back(current_time);
                current_time += processing_times[*it];
            }
        }
        int tardy_jobs_n = 0;
        int max_lateness = 0;
        int completion_time = start_points[gene.back()] + processing_times[gene.back()];
        for(int i = 0; i < gene.size(); i++){
            if(start_points[i] + processing_times[gene[i]] > due_dates[gene[i]]){
                tardy_jobs_n++;
                if(start_points[i] + processing_times[gene[i]] - due_dates[gene[i]] > max_lateness){
                    max_lateness = start_points[i] + processing_times[gene[i]] - due_dates[gene[i]];
                }
            }
        }
        return { (double) max_lateness, (double) tardy_jobs_n, (double) completion_time };
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