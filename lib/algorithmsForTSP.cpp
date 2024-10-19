#include "../include/algorithmsForTSP.h"

#include <iostream>

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::bruteForce(std::vector<std::vector<int32_t>> &graph)
{
	int32_t best_result = 0;

	std::vector<int32_t> best_route;

	for (int i = 0; i < graph.size() - 1; ++i)
	{
		best_route.push_back(i);
		best_result += graph[i][i + 1];
	}

	best_route.push_back(graph.size() - 1);
	best_result += graph[graph.size() - 1][0];

	std::vector<int32_t> route = best_route;

	algorithmsForTSP::permutate(route, 0, graph.size(), best_result, graph, best_route);

	return {best_result, best_route};
}

void algorithmsForTSP::permutate(std::vector<int32_t> &route, int32_t left, int32_t length, int32_t &current_result, std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &current_route)
{
	if (left == length)
	{
		int32_t result = 0;
		for (int i = 0; i < length - 1; ++i)
		{
			result += graph[route[i]][route[i + 1]];
		}

		result += graph[route[length - 1]][route[0]];

		if (result < current_result)
		{
			current_result = result;
			current_route = route;
		}

	}
	else
	{
		for (int i = left; i < length; ++i)
		{
			std::swap(route[i], route[left]);
			algorithmsForTSP::permutate(route, left + 1, length, current_result, graph, current_route);
			std::swap(route[i], route[left]);
		}
	}
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::nearestNeighbour(std::vector<std::vector<int32_t>> &graph)
{
	std::vector<int32_t> best_route;
	int32_t best_result = std::numeric_limits<int32_t>::max();

	for (int start_vertex = 0; start_vertex < graph.size(); ++start_vertex)
	{
		std::vector<int32_t> route;
		std::vector<bool> visited(graph.size(), false);
		route.push_back(start_vertex);
		visited[start_vertex] = true;
		int32_t result = 0;

		for (int i = route[0]; route.size() != graph.size(); i = route.back())
		{
			int32_t min = std::numeric_limits<int32_t>::max();
			int32_t min_i = -1;

			for (int j = 0; j < graph.size(); ++j)
			{
				if (visited[j])
					continue;

				if (graph[i][j] < min)
				{
					min = graph[i][j];
					min_i = j;
				}
			}

			route.push_back(min_i);
			visited[min_i] = true;
			result += min;
		}

		result += graph[route.back()][route[0]];

		if (result < best_result)
		{
			best_result = result;
			best_route = route;
		}
	}

	return {best_result, best_route};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::randomRoad(std::vector<std::vector<int32_t>> &graph)
{
	std::vector<int32_t> best_route;
	int32_t best_result = std::numeric_limits<int32_t>::max();

	for (int i = 0; i < graph.size() * graph.size(); ++i)
	{
		std::vector<bool> visited(graph.size(), false);

		std::vector<int32_t> route;
		int result = 0;

		randomEngine.seed(time(NULL));
		std::uniform_int_distribution<uint32_t> uint_dist_1_N(0, graph.size() - 1);

		while (route.size() != graph.size())
		{
			int32_t v = uint_dist_1_N(randomEngine);
			if (!visited[v])
			{
				route.push_back(v);
				visited[v] = true;
			}
		}

		for (int i = 0; i < route.size() - 1; ++i)
		{
			result += graph[route[i]][route[i + 1]];
		}

		result += graph[route.back()][route[0]];

		if (result < best_result)
		{
			best_result = result;
			best_route = route;
		}
	}

	return {best_result, best_route};
}

// 5
// 7
// 8
// 9
// 10
// 11
// 12