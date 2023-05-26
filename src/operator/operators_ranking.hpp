#include <functional>
#include <vector>
#include <random>
#include <cstdint>

using u32 = uint_least32_t;

// Ranking Operators-----------------------------------------------------------------

/*
    Pareto Ranking - Ranks vectors of fitness values based on pareto optimality
*/
template<typename T, typename L>
std::function<std::vector<int>(const std::vector<T>&)> rank_pareto(std::function<std::vector<L>(const std::vector<T>&)> evaluate) { 
    return [evaluate](const std::vector<T>& genes) -> std::vector<int> {
        auto dominates = ([](std::vector<double> p1, std::vector<double> p2) -> bool {
            for(int i = 0; i < p1.size(); i++){
                if(p1[i] < p2[i]){
                    return false;
                }
            }
            return true;
        });
        std::vector<std::vector<double>> fitnesses(genes.size());
        std::transform(genes.begin(), genes.end(), fitnesses.begin(), evaluate);
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