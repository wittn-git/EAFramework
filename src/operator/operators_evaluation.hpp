#include <functional>
#include <vector>
#include <random>
#include <cstdint>

using u32 = uint_least32_t; 

// Evaluation Operators -------------------------------------------------------------

/*
    Sum Evaluation: Assigns fitness values proportional to the sum of the genes chromosomes
*/

std::function<std::vector<double>(const std::vector<std::vector<int>>&)> evaluate_sum() { 
    return [](const std::vector<std::vector<int>>& genes) -> std::vector<double> {
        std::vector<double> fitnesses(genes.size());
        std::transform(genes.begin(), genes.end(), fitnesses.begin(), [](const std::vector<int>& gene) -> double {
            int sum = 0;
            for(auto it = gene.begin(); it != gene.end(); ++it){
                sum += *it;
            }
            return sum;
        });
        return fitnesses;
    };
};

/*
    TSP Evaluation: Assigns fitness according to the reciprocated tsp objective function
    Arguments:
        - distance_function:    Function taking two cities and returning the distance between them
*/

std::function<std::vector<double>(const std::vector<std::vector<int>>&)> evaluate_tsp(std::function<double(int x, int y)> distance_function) { 
    return [distance_function](const std::vector<std::vector<int>>& genes) -> std::vector<double> {
        std::vector<double> fitnesses(genes.size());
        std::transform(genes.begin(), genes.end(), fitnesses.begin(), [&](const std::vector<int>& gene) -> double {
            int sum = 0;
            for(auto it = gene.begin(); it != std::prev(gene.end()); ++it){
                sum += distance_function(*it, *std::next(it));
            }
            sum += distance_function(gene.back(), gene.front());
            return 1/(double)sum;
        });
        return fitnesses;
    };
};

/*
    Below3 Sum Evaluation: Assigns one fitness values proportional to the sum of the genes chromosomes and one based on the occurence of numbers below 3
*/

std::function<std::vector<std::vector<double>>(const std::vector<std::vector<int>>&)> evaluate_bel3sum() { 
    return [](const std::vector<std::vector<int>>& genes) -> std::vector<std::vector<double>> {
        std::vector<std::vector<double>> fitnesses(genes.size());
        std::transform(genes.begin(), genes.end(), fitnesses.begin(), [](const std::vector<int>& gene) -> std::vector<double> {
            int sum = 0;
            for(auto it = gene.begin(); it != gene.end(); ++it){
                sum += *it;
            }
            int below3_n = 0;
            for(auto it = gene.begin(); it != gene.end(); ++it){
                if(*it < 3) below3_n++;
            }
            return {(double) sum, (double) below3_n};
        });
        return fitnesses;
    };
}

/*
    Scheduling Evaluation: Evaluates Machine Jobs based on maximum lateness, number of tardy jobs and overall completion time
    Arguments:
        - processing_times: Times it takes to complete each job
        - release_dates:    Points in time where every job is released
        - due_dates:        Points in time where every job is due
*/

std::function<std::vector<std::vector<double>>(const std::vector<std::vector<int>>&)> evaluate_scheduling(std::vector<int> processing_times, std::vector<int> release_dates, std::vector<int> due_dates) { 
    return [processing_times, release_dates, due_dates](const std::vector<std::vector<int>>& gene) -> std::vector<std::vector<double>> {
        std::vector<std::vector<double>> fitnesses(gene.size());
        std::transform(gene.begin(), gene.end(), fitnesses.begin(), [&](const std::vector<int>& gene) -> std::vector<double> {
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
        });
        return fitnesses;
    };
}