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

## Installation

## Usage
### Creating a population
### Using predefined operators
### Defining operators

## Coming Soon
- filled out README
- compilation options as library
- more predefined operators
- testing utils
- plotting utils
- survivor selection
- generic encoding
- more efficient evaluation -> if multiple entities can be evaluated

## TODO
- rework random
- default arguments