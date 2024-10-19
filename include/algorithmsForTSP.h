#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <random>

class algorithmsForTSP
{
private:
	std::mt19937 randomEngine;

	void permutate(std::vector<int32_t> &route, int32_t left, int32_t length, int32_t &current_result, std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &current_route);

public:
	std::pair<int32_t, std::vector<int32_t>> bruteForce(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> nearestNeighbour(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> randomRoad(std::vector<std::vector<int32_t>> &graph);
};