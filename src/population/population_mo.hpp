#include "population.hpp"

// Class Outline ----------------------------------------------------------------------------------------------------------------------------

template<typename T>
class Population_MO : public Population<T>{

private:

    const std::function<std::vector<double>(const T&)>& evaluate;
    const std::function<std::vector<double>(const std::vector<std::vector<double>>&)>& rank;

    using Population<T>::genes;
    using Population<T>::size;
    using Population<T>::generator;
    using Population<T>::mutate;
    using Population<T>::recombine;
    using Population<T>::chooseParent;
    using Population<T>::select;

public:

    // Constructor for population initialized with passed genes initial_genes
    Population_MO(
        const std::vector<T>& initial_genes, 
        const std::function<std::vector<double>(const T&)>& evaluate,
        const std::function<T(const T&, std::mt19937&)>& mutate,
        const std::function<T(const T&, const T&, std::mt19937&)>& recombine,
        const std::function<std::vector<double>(const std::vector<std::vector<double>>&)>& rank,
        const std::function<T(const std::vector<double>&, const std::vector<T>& genes, std::mt19937&)>& chooseParent,
        const std::function<std::vector<T>(const std::vector<T>&, const std::vector<T>&)>& select,
        u32 seed
    );

   // Constructor for population of size size will with genes generated by function initialize
    Population_MO(
        int size,
        const std::function<T(std::mt19937&)>& initialize,
        const std::function<std::vector<double>(const T&)>& evaluate,
        const std::function<T(const T&, std::mt19937&)>& mutate,
        const std::function<T(const T&, const T&, std::mt19937&)>& recombine,
        const std::function<std::vector<double>(const std::vector<std::vector<double>>&)>& rank,
        const std::function<T(const std::vector<double>&, const std::vector<T>&, std::mt19937&)>& chooseParent,
        const std::function<std::vector<T>(const std::vector<T>&, const std::vector<T>&)>& select,
        u32 seed
    );

    void execute(bool useRecombination, bool useMutation); //executes one iteration of the evolutionary algorithm
    std::set<T> getBests(); //returns the genes with the highest rank in the current population
};

// Class Implementation ---------------------------------------------------------------------------------------------------------------------

template<typename T>
Population_MO<T>::Population_MO(
    const std::vector<T>& initial_genes, 
    const std::function<std::vector<double>(const T&)>& evaluate,
    const std::function<T(const T&, std::mt19937&)>& mutate,
    const std::function<T(const T&, const T&, std::mt19937&)>& recombine,
    const std::function<std::vector<double>(const std::vector<std::vector<double>>&)>& rank,
    const std::function<T(const std::vector<double>&, const std::vector<T>& genes, std::mt19937&)>& chooseParent,
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<T>&)>& select,
    u32 seed
) : Population<T>(initial_genes, seed, mutate, recombine, chooseParent, select), evaluate(evaluate), rank(rank) {}

template<typename T>
Population_MO<T>::Population_MO(
    int size,
    const std::function<T(std::mt19937&)>& initialize,
    const std::function<std::vector<double>(const T&)>& evaluate,
    const std::function<T(const T&, std::mt19937&)>& mutate,
    const std::function<T(const T&, const T&, std::mt19937&)>& recombine,
    const std::function<std::vector<double>(const std::vector<std::vector<double>>&)>& rank,
    const std::function<T(const std::vector<double>&, const std::vector<T>&, std::mt19937&)>& chooseParent,
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<T>&)>& select,
    u32 seed
) : Population<T>(size, initialize, seed, mutate, recombine, chooseParent, select), evaluate(evaluate), rank(rank) {}

template<typename T>
void Population_MO<T>::execute(bool useRecombination, bool useMutation) {
    std::vector<std::vector<double>> fitnessValues(size);
    for(int j = 0; j < size; j++){
        fitnessValues[j] = evaluate(genes[j]);
    }
    std::vector<T> new_genes = {};
    std::vector<double> ranks = rank(fitnessValues);
    for(int j = 0; j < size; j++){        
        T parent1 = chooseParent(ranks, genes, generator);
        T child = parent1;
        if(useRecombination){
            T parent2 = chooseParent(ranks, genes, generator);
            child = recombine(parent1, parent2, generator);
        }
        if(useMutation){
            child = mutate(child, generator);
        }
        new_genes.emplace_back(child);
    }
    genes = new_genes;
}

template<typename T>
std::set<T> Population_MO<T>::getBests(){
    std::vector<std::vector<double>> fitnessValues(size);
    for(int i = 0; i < size; i++){
        fitnessValues[i] = evaluate(genes[i]);
    }
    std::vector<double> ranks = rank(fitnessValues);
    std::set<T> bests;
    for(int i = 0; i < size; i++){
        if(ranks[i] == 1) bests.emplace(genes[i]);
    }
    return bests;
}