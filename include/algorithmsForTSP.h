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
public:
	std::pair<int32_t, std::vector<int32_t>> genetic_algorithm(std::vector<std::vector<int32_t>>& graph, int crossing_strategy, int mutation_stategy);
};