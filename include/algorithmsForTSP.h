#pragma once
#include <vector>
#include <stack> 
#include <queue>
#include <cstdint>
#include <limits>
#include <random>
#include <utility>

class Node {
public:
    int32_t current_vertex;
    int32_t lower_bound;
    int32_t cost;
    uint32_t visited_bits;
    int32_t* current_path;
    int current_path_length;
    int matrix_size;

    Node(int matrix_size);
    Node(const Node& other);
	Node(Node&& other) noexcept;
    Node& operator=(const Node& other);
	Node& operator=(Node&& other) noexcept;
	// Overload the operator< for priority_queue
    bool operator<(const Node& other) const;

    ~Node();
};



class algorithmsForTSP
{
private:
	std::mt19937 randomEngine;

	int32_t calculateLowerBound(const std::vector<std::vector<int32_t>>& graph, const Node& node, int32_t start_vertex);
	void permutate(std::vector<int32_t> &route, int32_t left, int32_t length, int32_t &current_result, std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &current_route);
public:
    //Stage 1

    std::pair<int32_t, std::vector<int32_t>> nearestNeighbour(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> bruteForce(std::vector<std::vector<int32_t>> &graph);
	std::pair<int32_t, std::vector<int32_t>> randomRoad(std::vector<std::vector<int32_t>> &graph);

	// Stage 2

	std::pair<int32_t, std::vector<int32_t>> dfs(std::vector<std::vector<int32_t>> &graph, bool isDirected);
	std::pair<int32_t, std::vector<int32_t>> bfs(std::vector<std::vector<int32_t>> &graph, bool isDirected);
	std::pair<int32_t, std::vector<int32_t>> best_first_search(std::vector<std::vector<int32_t>> &graph, bool isDirected);
};