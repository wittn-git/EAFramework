#pragma once

#include <vector>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <random>
#include <cctype>
#include <random>
#include <assert.h>

// Class Outline ----------------------------------------------------------------------------------------------------------------------------

// Class for a population of genes of type T, with fitness values of type L
template<typename T, typename L>
class Population{

private:

    std::vector<T> genes;
    std::mt19937 generator;

    // Function taking a vector of genes of type T and returning its fitness value vector of type L
    const std::function<std::vector<L>(const std::vector<T>&)>& evaluate;
    // Function taking a vector of genes of type T and returning a vector of parents of type T
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents;
    // Function taking a vector of genes of type T and returning a vector of mutated genes of type T
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate;
    // Function taking a vector of genes of type T and returning a vector of recombined genes of type T
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine;
    // Function taking two vectors of genes of type T (parents and children) and returning a selected vector of genes of type T
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors;
    // Function taking a vector of fitness values of type L and returning a vector of ranks of type int
    const std::function<std::vector<int>(const std::vector<L>&)>& rank;

    void check_types();

public:

    // Constructor for population initialized with passed genes initial_genes
    Population(
        int seed,
        const std::vector<T>& initial_genes,
        const std::function<std::vector<L>(const std::vector<T>&)>& evaluate,
        const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents,
        const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate = nullptr,
        const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine = nullptr,
        const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors = nullptr,
        const std::function<std::vector<int>(const std::vector<L>&)>& rank = nullptr
    );

    // Constructor for population of size size will with genes generated by function initialize
    Population(
        int seed,
        const std::function<std::vector<T>(std::mt19937&)>& initialize,
        const std::function<std::vector<L>(const std::vector<T>&)>& evaluate,
        const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents,
        const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate = nullptr,
        const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine = nullptr,
        const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors = nullptr,
        const std::function<std::vector<int>(const std::vector<L>&)>& rank = nullptr
    );

    void execute(); //executes one iteration of the evolutionary algorithm
    void execute_multiple(int generations); //executes 'generations' iterations of the evolutionary algorithm
    std::vector<T> get_bests(bool keep_duplicats); //returns the best genes in the population
    std::vector<T> get_genes(); //returns the current genes in the population
    void set_genes(std::vector<T> genes); //sets the genes of the population
    std::string to_string(); //returns a string representation of the population

    void set_evaluate(const std::function<std::vector<L>(const std::vector<T>&)>& evaluate);
    void set_selectParents(const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents);
    void set_mutate(const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate);
    void set_recombine(const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine);
    void set_selectSurvivors(const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors);
    void set_rank(const std::function<std::vector<int>(const std::vector<L>&)>& rank);

};

// Class Implementation ---------------------------------------------------------------------------------------------------------------------

template<typename T, typename L>
Population<T, L>::Population(
    int seed,
    const std::vector<T>& initial_genes,
    const std::function<std::vector<L>(const std::vector<T>&)>& evaluate,
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents,
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate,
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine,
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors,
    const std::function<std::vector<int>(const std::vector<L>&)>& rank
) : generator(seed), genes(initial_genes), evaluate(evaluate), selectParents(selectParents), mutate(mutate), recombine(recombine), selectSurvivors(selectSurvivors), rank(rank) {
    check_types();
}

template<typename T, typename L>
Population<T, L>::Population(
    int seed,
    const std::function<std::vector<T>(std::mt19937&)>& initialize,
    const std::function<std::vector<L>(const std::vector<T>&)>& evaluate,
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents,
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate,
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine,
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors,
    const std::function<std::vector<int>(const std::vector<L>&)>& rank
) : generator(seed), evaluate(evaluate), selectParents(selectParents), mutate(mutate), recombine(recombine), selectSurvivors(selectSurvivors), rank(rank) {
    check_types();
    genes = initialize(generator);
}

template<typename T, typename L>
void Population<T, L>::check_types() {
    assert((evaluate != nullptr && selectParents != nullptr));
}

template<typename T, typename L>
void Population<T, L>::execute() {
    std::vector<L> fitnesses = evaluate(genes);
    std::vector<int> ranks = (rank == nullptr) ? std::vector<int>() : rank(fitnesses);
    std::vector<T> parents = selectParents(genes, fitnesses, ranks, generator);
    std::vector<T> children = (recombine == nullptr) ? parents : recombine(parents, generator);
    children = (mutate == nullptr) ? children : mutate(children, generator);
    genes = (selectSurvivors == nullptr) ? children : selectSurvivors(genes, fitnesses, ranks, children, generator);
}

template<typename T, typename L>
void Population<T, L>::execute_multiple(int generations){
    for(int i = 0; i < generations; i++){
        execute();
    }
}

template<typename T, typename L>
std::vector<T> Population<T, L>::get_bests(bool keep_duplicats){
    std::vector<T> bests;
    std::vector<L> fitnesses = evaluate(genes);
    if(rank != nullptr){
        std::vector<int> ranks = rank(fitnesses);
        for(int i = 0; i < genes.size(); i++){
            if(ranks[i] == 1){
                bests.emplace_back(genes[i]);
            }
        }
    }else{
        auto max_it = std::max_element(fitnesses.begin(), fitnesses.end());
        for(int i = 0; i < genes.size(); i++){
            if(fitnesses[i] == *max_it){
                bests.emplace_back(genes[i]);
            }
        }
    }
    std::sort(bests.begin(), bests.end());
    if(keep_duplicats){
        return bests;
    }
    bests.erase(std::unique(bests.begin(), bests.end()), bests.end());
    return bests;
}

template<typename T, typename L>
std::vector<T> Population<T, L>::get_genes(){
    return genes;
}

template<typename T, typename L>
void Population<T, L>::set_genes(std::vector<T> genes){
    this.genes = genes;
}

template<typename T, typename L>
std::string Population<T, L>::to_string(){
    std::string s;
    for (auto gene : genes) {
        s += std::to_string(gene) + "\n";
    }
    return s;
}

template<typename T, typename L>
void Population<T, L>::set_evaluate(const std::function<std::vector<L>(const std::vector<T>&)>& evaluate){ this.evaluate = evaluate;}
template<typename T, typename L>
void Population<T, L>::set_selectParents(const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents){ this.selectParents = selectParents;}
template<typename T, typename L>
void Population<T, L>::set_mutate(const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate){ this.mutate = mutate;}
template<typename T, typename L>
void Population<T, L>::set_recombine(const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine){ this.recombine = recombine;}
template<typename T, typename L>
void Population<T, L>::set_selectSurvivors(const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors){ this.selectSurvivors = selectSurvivors;}
template<typename T, typename L>
void Population<T, L>::set_rank(const std::function<std::vector<int>(const std::vector<L>&)>& rank){ this.rank = rank;}