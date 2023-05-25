#include <functional>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <utility>
#include <ranges>
#include <set>
#include <iostream>
#include <random>

using u32 = uint_least32_t; 

// Mutation Operators ---------------------------------------------------------------

/*
    Sigma Block Mutation: Take a random point and the block of 2 times sigma around it, then sort it according to the EDD rule (earliest due date first)
*/

std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)> mutate_sigmablock(double mutation_rate, int sigma, std::vector<int> due_dates) {
    return [mutation_rate, sigma, due_dates](const std::vector<int>& gene, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> mutated_gene = gene;
        std::uniform_real_distribution< double > distribute_rate(0, 1);
        std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 2*sigma - 1 );
        if (distribute_rate(generator) < mutation_rate) {
            int midpoint = sigma + distribute_point(generator);
            std::vector<int> block(mutated_gene.begin() + midpoint - sigma, mutated_gene.begin() + midpoint + sigma + 1);
            std::sort(block.begin(), block.end(), [due_dates](int a, int b) {
                return due_dates[a] < due_dates[b];
            });
            std::copy(block.begin(), block.end(), mutated_gene.begin() + midpoint - sigma);
        }
        return mutated_gene;
    };
}

/*
    Sigma Block Mutation: Take sigma random positions, then sort it according to the EDD rule (earliest due date first)
*/
std::function<std::vector<int>(const std::vector<int>&, std::mt19937&)> mutate_extsigmablock(double mutation_rate, int sigma, std::vector<int> due_dates) {
    return [mutation_rate, sigma, due_dates](const std::vector<int>& gene, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> mutated_gene = gene;
        std::uniform_real_distribution< double > distribute_rate(0, 1);
        std::uniform_int_distribution< u32 > distribute_point(0, gene.size() - 1);
        if (distribute_rate(generator) < mutation_rate) {
            std::vector<int> indices, points;
            for(int i = 0; i < sigma; i++){
                int index = distribute_point(generator);
                indices.emplace_back(index);
                points.emplace_back(gene[index]);
            }
            std::sort(points.begin(), points.end(), [due_dates](int a, int b) {
                return due_dates[a] < due_dates[b];
            });
            for(int i = 0; i < sigma; i++){
                mutated_gene[indices[i]] = points[i];
            }
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

std::function<std::vector<int>(const std::vector<double>&, const std::vector<std::vector<int>>&, std::mt19937&)> select_tournament_rank(int tournament_size) {
    return [tournament_size](const std::vector<double>& ranks, const std::vector<std::vector<int>>& genes, std::mt19937& generator) -> std::vector<int> {
        std::vector<int> selected_genes(tournament_size);
        std::uniform_int_distribution< u32 > distribute_point(0, genes.size() - 1);
        for (int i = 0; i < tournament_size; i++) {
            int rand_index = distribute_point(generator);
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
        int completion_time = start_points.back() + processing_times[gene.back()];
        for(int i = 0; i < gene.size(); i++){
            if(start_points[i] + processing_times[gene[i]] > due_dates[gene[i]]){
                tardy_jobs_n++;
                if(start_points[i] + processing_times[gene[i]] - due_dates[gene[i]] > max_lateness){
                    max_lateness = start_points[i] + processing_times[gene[i]] - due_dates[gene[i]];
                }
            }
        }
        return { (double) 1/max_lateness, (double) 1/tardy_jobs_n, (double) 1/completion_time };
    };
}

// Ranking Operators-----------------------------------------------------------------

/*
    Pareto Ranking - Ranks vectors of fitness values based on pareto optimality
*/
std::function<std::vector<double>(const std::vector<std::vector<double>>&)> rank_pareto() { 
    return [](const std::vector<std::vector<double>>& fitnesses) -> std::vector<double> {
        auto dominates = ([](std::vector<double> p1, std::vector<double> p2) -> bool {
            for(int i = 0; i < p1.size(); i++){
                if(p1[i] < p2[i]){
                    return false;
                }
            }
            return true;
        });
        std::vector<double> ranks(fitnesses.size(), 0);
        std::vector<int> n(fitnesses.size(), 0);
        std::vector<std::vector<int>> S(fitnesses.size(), std::vector<int>{});
        std::vector<std::vector<int>> F = {{}};
        for(int i = 0; i < fitnesses.size(); i++){
            for(int j = 0; j < fitnesses.size(); j++){
                if(i == j) continue;
                if(dominates(fitnesses[i], fitnesses[j])){
                    S[i].push_back(j);
                } else if(dominates(fitnesses[j], fitnesses[i])){
                    n[i]++;
                }
            }
            if(n[i] == 0){
                ranks[i] = 1;
                F[0].push_back(i);
            }
        }
        int i = 0;
        while(!F[i].empty()){
            std::vector<int> Q = {};
            for(auto p : F[i]){
                for(auto q : S[p]){
                    n[q]--;
                    if(n[q] == 0){
                        ranks[q] = i + 2;
                        Q.push_back(q);
                    }
                }
            }
            i++;
            F.push_back(Q);
        }
        return ranks;
    };
}

// Offspring Selection Operators ----------------------------------------------------

/*
    Offspring Selection - Selects just offpsring as offspring 
*/

std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&)> select_offspring() { 
    return [](const std::vector<std::vector<int>>& parents, const std::vector<std::vector<int>>& offspring) -> std::vector<std::vector<int>> {
        return offspring;
    };
}