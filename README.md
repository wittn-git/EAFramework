# EAFramework
General framework for Evolutionary Algorithms.

## Overview

This repository serves as a general framework for Evolutionary Algorithms and can be tailored to solve a range of optimization problems.
Evolutionary Algorithms are a nature-inspired, stochastic approach of optimization. They work be maintaining a population of possible solutions, whereas after each generation the most fit individuals are allowed to reproduce. This offspring will be mutated to allow for new solutions. After a number of generation, some time has passed or after some other stopping criterion, the algorithm will output one or multiple solutions (usally of high quality or even optimal).
The process usally follows the same pattern:
- Initialization of the population
- Until some stopping criterion is reached:
    - Evaluate individiuals
    - Recombination
    - Mutation
    - Survivor Selection

This repository implements this general process while allowing for costum operators for encoding, evaluation, recombination, mutation and survivor selection.

## Usage
### Creating a population
To define a population, include the "population.hpp" file in your project. Every population has the same basic building blocks:
- Encoding: Type of the individuals (T) and type of their fitness values (L)
```c++
Population<T, L> population;
```
For example, if each solution is a vector of strings and the fitness value is a double, the population can be initialized as follows:
```c++
Population<std::vector<std::string>, double> population;
```
- Initialization: A function that returns a vector of individuals OR a vector of individuals
```c++
std::vector<T> initialization(std::mt19937&);
```
OR
```c++
std::vector<T> initial_genes;
```

- Evaluation: A function that takes a vector of individuals and returns a vector of fitness values
```c++
std::vector<L> evaluation(const std::vector<T>&);
```
- Parent Selection: A function that takes a vector of individuals, a vector of their fitness values, a vecor of their ranks and a random generator  and returns a vector of selected individuals
```c++
std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> parent_selection;
```
- Mutation: A function that takes a vector of individuals and a random generator and returns a vector of mutated individuals (optinonal)
```c++
std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> mutate;
```
- Recombination: A function that takes a vector of individuals and a random generator and returns a vector of recombined individuals
```c++
std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)> recombine;
```
- Survivor Selection: A function that takes a vector of individuals (parents), a vector of their fitness values, a vecor of their ranks, another vector of individuals (children) and a random generator  and returns a vector of selected individuals (optional)
```c++
std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)> survivor_selection;
```
- Ranking: A function that takes a vector of fitness values and returns a vector of ranks (optional)
```c++
std::function<std::vector<int>(const std::vector<L>&)> rank;
```
All these function can than be passed to the constructor, e.g.:
```c++
Population<std::vector<std::string>, double> population(initialization, evaluation, parent_selection, recombine, mutate, survivor_selection, rank);
```
The function which are marked with "optional" can be left out and nullptr can be passed instead. If this is done, the according operator will not be used (e.g. if no mutation operator is passed, the population will not be mutated).

### Using predefined operators

Some operators are already predefined and can be used without defining them yourself. They can be used by including the according header file. The header files are:
- "operators/operators_initialization.hpp" - Initialization operators
- "operators/operators_evaluation.hpp" - Evaluation operators
- "operators/operators_parent_selection.hpp" - Parent selection operators
- "operators/operators_recombination.hpp" - Recombination operators
- "operators/operators_mutation.hpp" - Mutation operators
- "operators/operators_survivor_selection.hpp" - Survivor selection operators
- "operators/operators_ranking.hpp" - Ranking operators

The operators can be used by calling the according function (with possible additional parameters). For example:
```c++
std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate = mutate_swap(mutation_rate)
```
Note that some operators require additional template initialization, if the types can not be deduced from the parameters. For example:
```c++
using T = std::vector<int>;
using L = double;
std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_parents_so = select_tournament<T, L>(4);
```

 The following operators are already implemented:
 Initialization:
    - Random initialization: Randomly initialize a gene of length gene_length with chromosomes from chromosome_list
    ```c++
    std::function<std::vector<std::vector<int>>(std::mt19937&)> initialize_random(int population_size, int gene_length, const std::vector<int> chromosome_list);
    ```
    - Random Permutation Initialization: Randomly initialize a gene with a permutation of chromosome_list
    ```c++
    std::function<std::vector<std::vector<int>>(std::mt19937&)> initialize_random_permutation(int population_size, const std::vector<int> chromosome_list);
    ```
Evaluation:
    - Sum Evaluation: Assigns fitness values proportional to the sum of the genes chromosomes
    ```c++
    std::function<std::vector<double>(const std::vector<std::vector<int>>&)> evaluate_sum();
    ```
    - TSP Evaluation: Assigns fitness according to the reciprocated tsp objective function
    ```c++
    std::function<std::vector<double>(const std::vector<std::vector<int>>&)> evaluate_tsp(std::function<double(int x, int y)> distance_function);
    ```
    - Below3 Sum Evaluation: Assigns one fitness values proportional to the sum of the genes chromosomes and one based on the occurence of numbers below 3
    ```c++
    std::function<std::vector<double>(const std::vector<std::vector<int>>&)> evaluate_below3();
    ```
    - Scheduling Evaluation: Evaluates Machine Jobs based on maximum lateness, number of tardy jobs and overall completion time
    ```c++
    std::function<std::vector<std::vector<double>>(const std::vector<std::vector<int>>&)> evaluate_scheduling(std::vector<int> processing_times, std::vector<int> release_dates, std::vector<int> due_dates);
    ```
Parent Selection:
    - Tournament Parent Selection: Take a random subgroup of a specified size and choose the one with the best rank
    ```c++
    template<typename T, typename L>
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_tournament(int tournament_size);
    ```
    - Roulette Parent Selection: Select parents with a probability proportional to their fitness
    ```c++
    template<typename T, typename L>
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_roulette();
    ```
    - Tournament Parent Selection: Take a random subgroup of a specified size and choose the one with the highest fitness value
    ```c++
    template<typename T, typename L>
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)> select_tournament(int tournament_size);
    ```
Mutation:
    - Numeric Mutation: Increment or decrement one chromosome by one
    ```c++
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_numeric(double mutation_rate, double incrementation_rate, int chromosome_max, int chromosome_min);
    ```
    - Swap Mutation: Swap two chromosomes
    ```c++
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_swap(double mutation_rate);
    ```
    - Sigma Block Mutation: Take a random point and the block of 2 times sigma around it, then sort it according to the EDD rule (earliest due date first)
    ```c++
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_sigmablock(double mutation_rate, int sigma, std::vector<int> due_dates)
    ```
    - Extended Sigma Block Mutation: Take sigma random positions, then sort it according to the EDD rule (earliest due date first)
    ```c++
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> mutate_extsigmablock(double mutation_rate, int sigma, std::vector<int> due_dates)
    ```
Recombination:
    - Midpoint Recombination: Takes the first half of the gene of the first parents gene and the second half of the second parents gene to create a new gene, parents are the pairs of genes iterating the list of genes
    ```c++
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> recombine_midpoint();
    ```
    - Twopoint Recombination: Takes a segment of the first parent and adds it to the childs gene. Then it fills out the residual chromosomes in the order of the second parent.
    ```c++
    std::function<std::vector<std::vector<int>>(const std::vector<std::vector<int>>&, std::mt19937&)> recombine_twopoint();
    ```
Survivor Selection:
    - muh-Selection: Selects the best muh individuals from the combined population of parents and offspring
    ```c++
    template<typename T, typename L>
    std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)> select_muh(int muh);
    ```
Ranking:
    - Pareto Ranking - Ranks vectors of fitness values based on pareto optimality
    ```c++
    std::function<std::vector<int>(const std::vector<std::vector<double>>&)> rank_pareto();
    ```

### Defining operators
It is also possible to define operators costumarily. To do so, the according function signature has to be followed. 
```c++
    const std::function<std::vector<T>(std::mt19937&)>& initialize;
    const std::function<std::vector<L>(const std::vector<T>&)>& evaluate;
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, std::mt19937&)>& selectParents;
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& mutate
    const std::function<std::vector<T>(const std::vector<T>&, std::mt19937&)>& recombine;
    const std::function<std::vector<T>(const std::vector<T>&, const std::vector<L>&, const std::vector<int>&, const std::vector<T>&, std::mt19937&)>& selectSurvivors;
    const std::function<std::vector<int>(const std::vector<L>&)>& rank;
```
where T is the type of the individuals and L is the type of the fitness values. The functions can then be passed to the constructor of the population. If the costum function needs additional parameters, they can be passed during the initialization of the function. 

For example, if a costum evaluation function is needed, it can be defined as follows:
```c++
std::function<std::vector<double>(const std::vector<std::vector<int>>&)> evaluate_costum(int parameter1, double parameter2) { 
    return [](const std::vector<std::vector<int>>& genes) -> std::vector<double> {
        std::vector<double> fitnesses(genes.size());
        std::transform(genes.begin(), genes.end(), fitnesses.begin(), [](const std::vector<int>& gene) -> double {
            std::vector<int> altered_gene = gene;
            altered_gene[parameter1] = altered_gene[parameter1] * parameter2;
            int sum = 0;
            for(auto it = altered_gene.begin(); it != altered_gene.end(); ++it){
                sum += *it;
            }
            return sum;
        });
        return fitnesses;
    };
};
```
which defines a costum evaluation function that takes two fixed parameters and multiplies the chromosome at position parameter1 with parameter2. It can then be initialized as follows:
```c++
std::function<std::vector<double>(const std::vector<std::vector<int>>&)> evaluate = evaluate_costum(2, 0.5);
```
and passed to the constructor of the population:
```c++
Population<std::vector<int>, double> population(initialize, evaluate, selectParents, recombine, mutate, selectSurvivors, rank);
```