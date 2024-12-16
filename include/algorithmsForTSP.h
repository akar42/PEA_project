#pragma once
#include <vector>
#include <stack> 
#include <queue>
#include <cstdint>
#include <limits>
#include <random>
#include <utility>

class algorithmsForTSP
{
public:
    std::pair<int32_t, std::vector<int32_t>> tabu_search(std::vector<std::vector<int32_t>>& graph);
    std::pair<int32_t, std::vector<int32_t>> simulated_annealing(std::vector<std::vector<int32_t>>& graph);
};