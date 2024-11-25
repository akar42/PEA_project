#include <iostream>
#include <windows.h>
#include <vector>
#include "./include/graphCreator.h"
#include "./include/algorithmsForTSP.h"
#include <chrono>

int main()
{
	std::cout << "Hello world!" << '\n';

	graphCreator graphCreator;

	// std::vector<std::vector<int32_t>> graph = graphCreator.readGraphFromFile("graph_n_8.txt");
	std::vector<std::vector<int32_t>> graph = graphCreator.generateRandomGraph(10, false);

	std::cout << "V_i\\V_j\t| ";

	for (int j = 0; j < graph.size(); j++)
	{
		std::cout << j << '\t';
	}

	std::cout << '\n';

	for (int i = 0; i < 8 * (graph.size() + 1); i++)
	{
		if (i == 8)
			std::cout << '+';
		else
			std::cout << '-';
	}

	std::cout << '\n';

	for (int i = 0; i < graph.size(); ++i)
	{
		std::cout << i << "\t| ";

		for (int j = 0; j < graph[i].size(); ++j)
		{
			std::cout << graph[i][j] << '\t';
		}
		std::cout << '\n';
	}

	algorithmsForTSP algs;
	int32_t start_vertex = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
	std::chrono::duration<double, std::milli> delta;

	start = std::chrono::high_resolution_clock::now();
	std::pair<int32_t, std::vector<int32_t>> res_bfs = algs.bfs(graph, true, start_vertex);
	end = std::chrono::high_resolution_clock::now();

	delta = end - start;

	std::cout << "BFS - Execution time: " << delta.count() << '\n';

	for (int32_t vertex : res_bfs.second)
	{
		std::cout << vertex << " - ";
	}

	std::cout << res_bfs.second[0] << '\n';
	std::cout << res_bfs.first << '\n';

	start = std::chrono::high_resolution_clock::now();
	std::pair<int32_t, std::vector<int32_t>> res_dfs = algs.dfs(graph, true, start_vertex);
	end = std::chrono::high_resolution_clock::now();

	delta = end - start;

	std::cout << "DFS - Execution time: " << delta.count() << '\n';

	for (int32_t vertex : res_dfs.second)
	{
		std::cout << vertex << " - ";
	}

	std::cout << res_dfs.second[0] << '\n';
	std::cout << res_dfs.first << '\n';

	start = std::chrono::high_resolution_clock::now();
	std::pair<int32_t, std::vector<int32_t>> res_best_first = algs.best_first_search(graph, true, start_vertex);
	end = std::chrono::high_resolution_clock::now();

	delta = end - start;

	std::cout << "Best First - Execution time: " << delta.count() << '\n';

	for (int32_t vertex : res_best_first.second)
	{
		std::cout << vertex << " - ";
	}

	std::cout << res_best_first.second[0] << '\n';
	std::cout << res_best_first.first << '\n';
	// return 0;
}
// graph_n_6 - 150
// graph_n_8 - 136
// graph_n_11 - 149