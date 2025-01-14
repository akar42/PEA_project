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

class algorithmsForTSP
{
private:
	std::mt19937 randomEngine;

	std::pair<int32_t, std::vector<int32_t>> nearestNeighbour(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> greedyEstimation(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> calculateNeighbour_Swap(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> route, int32_t city1, int32_t city2);
	std::pair<int32_t, std::vector<int32_t>> calculateNeighbour_2Opt(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t>& route, int32_t city1, int32_t city2);
	std::pair<int32_t, std::vector<int32_t>> calculateNeighbour_Insert(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t>& route, int32_t city1, int32_t city2);
	std::pair<int32_t, std::vector<int32_t>> choose_neighbour(std::vector<std::vector<int32_t>> &graph,
															  std::pair<int32_t, std::vector<int32_t>> &current, 
															  std::vector<std::vector<int32_t>> &tabu_list,
															  int32_t &neighbour_strategy,
															  int32_t best_cost);
	void simulated_annealing(std::vector<std::vector<int32_t>> &graph,
							 double alpha,
							 std::shared_ptr<std::pair<int32_t,std::vector<int32_t>>> current_result,
							 std::atomic<bool> &stop_flag, 
							 double &execution_time,
							 bool logs);
	void tabu_search(std::vector<std::vector<int32_t>> &graph,
					 int neighbour_strategy,
					 std::shared_ptr<std::pair<int32_t, std::vector<int32_t>>> current_result,
					 std::atomic<bool> &stop_flag,
					 double &execution_time,
					 bool logs);

public:
	std::pair<int32_t, std::vector<int32_t>> sa_with_time_limit(std::vector<std::vector<int32_t>> &graph,
																double alpha,
																int time_limit_seconds,
																double &execution_time,
																bool logs);
	std::pair<int32_t, std::vector<int32_t>> ts_with_time_limit(std::vector<std::vector<int32_t>> &graph,
																int neighbour_strategy,
																int time_limit_seconds,
																double &execution_time,
																bool logs);
};