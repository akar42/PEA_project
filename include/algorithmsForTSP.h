#pragma once
#include <vector>
#include <stack>
#include <queue>
#include <cstdint>
#include <limits>
#include <random>
#include <utility>
#include <iostream>
#include <cmath>
#include <fstream>
#include <thread>
#include <future>
#include <chrono>
#include <atomic>
#include <memory>
#include <algorithm>
#include <set>
#include <unordered_map>

class algorithmsForTSP
{
private:
	std::mt19937 randomEngine;
	
	std::pair<int32_t, std::vector<int32_t>> nnRouteFromVertex(std::vector<std::vector<int32_t>> &graph, int32_t start_vertex);
	std::pair<int32_t, std::vector<int32_t>> randomRoute(std::vector<std::vector<int32_t>> &graph);

	std::vector<std::pair<int32_t, std::vector<int32_t>>> initializeColony(std::vector<std::vector<int32_t>> &graph, int32_t n);

	std::pair<std::vector<int32_t>, std::vector<int32_t>> pmx_crossover(std::vector<int32_t> &parent1, std::vector<int32_t> &parent2);
	std::pair<std::vector<int32_t>, std::vector<int32_t>> ox_crossover(std::vector<int32_t> &parent1, std::vector<int32_t> &parent2);

	std::pair<int32_t, std::vector<int32_t>> calculateNeighbour_Swap(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &route, int32_t city1, int32_t city2);
	std::pair<int32_t, std::vector<int32_t>> calculateNeighbour_Insert(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &route, int32_t city1, int32_t city2);

public:
	algorithmsForTSP();
	std::pair<int32_t, std::vector<int32_t>> genetic_algorithm(std::vector<std::vector<int32_t>> &graph, int32_t colony_size, int32_t crossing_strategy, int32_t mutation_stategy, float crossover_rate, float mutation_rate, float time_limit);
};