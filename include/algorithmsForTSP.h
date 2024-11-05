#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <random>

class algorithmsForTSP
{
public:
	std::pair<int32_t, std::vector<int32_t>> dfs(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> best_first_search(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> ida(std::vector<std::vector<int32_t>> &graph);
};