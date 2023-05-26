#include <functional>
#include <vector>
#include <random>
#include <cstdint>

using u32 = uint_least32_t;

// Recombination Operators ----------------------------------------------------------

/*
    Midpoint Recombination: Takes the first half of the gene of the first parents gene and the second half of the second parents gene to create a new gene, parents are the pairs of genes iterating the list of genes
*/

std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> recombine_midpoint() { 
    return [](const std::vector<std::vector<int>>& parents, std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> children(parents.size());
        int procreations_n = parents.size() % 2 == 0 ? parents.size() : parents.size()-1;
        for(int i = 0; i < procreations_n; i+=2){
            std::vector<int> parent1 = i % 2 == 0 ? parents[i] : parents[i-1];
            std::vector<int> parent2 = i % 2 == 0 ? parents[i+1] : parents[i];
            std::vector<int> child(parent1.size());
            int midpoint = parent1.size() / 2;
            std::copy(parent1.begin(), parent1.begin() + midpoint, child.begin());
            std::copy(parent2.begin() + midpoint, parent2.end(), child.begin() + midpoint);
            children[i] = child;
        }
        if(parents.size() % 2 != 0){
            children[parents.size()-1] = parents[parents.size()-1];
        }
        return children;
    };
};

/*
    Twopoint Recombination: Takes a segment of the first parent and adds it to the childs gene. Then it fills out the residual chromosomes in the order of the second parent.
*/

std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> recombine_twopoint() { 
    return [](const std::vector<std::vector<int>>& parents, std::mt19937& generator) -> std::vector<std::vector<int>> {
        std::vector<std::vector<int>> children(parents.size());
        int procreations_n = parents.size() % 2 == 0 ? parents.size() : parents.size()-1;
        for(int i = 0; i < procreations_n; i+=2){
            std::vector<int> parent1 = i % 2 == 0 ? parents[i] : parents[i-1];
            std::vector<int> parent2 = i % 2 == 0 ? parents[i+1] : parents[i];
            std::vector<int> child(parent1.size());
            std::uniform_real_distribution< double > distribute_point1(0, parent1.size() - 2);
            int i1 = distribute_point1(generator);
            std::uniform_real_distribution< double > distribute_point2(0, parent1.size() - i1 - 2);
            int i2 = distribute_point2(generator) + i1;
            std::vector<int> segment(parent1.begin() + i1, parent1.begin() + i2);
            int j = 0;
            while(child.size() < i1){
                if(std::find(segment.begin(), segment.end(), parent2[j]) == segment.end() && std::find(child.begin(), child.end(), parent2[j]) == child.end()){
                    child.push_back(parent2[j]);
                }
                j++;
            }
            child.insert(child.end(), segment.begin(), segment.end());
            while(child.size() < parent1.size()){
                if(std::find(child.begin(), child.end(), parent2[j]) == child.end()){
                    child.push_back(parent2[j]);
                }
                j++;
            }
            children[i] = child;
        }
        if(parents.size() % 2 != 0){
            children[parents.size()-1] = parents[parents.size()-1];
        }
        return children;
    };
};