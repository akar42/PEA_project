#include <iostream>
#include <windows.h>
#include <vector>
#include "./include/graphCreator.h"
#include "./include/algorithmsForTSP.h"

int main()
{
	std::cout << "Hello world!" << '\n';

	graphCreator graphCreator;

	std::vector<std::vector<int32_t>> graph = graphCreator.readGraphFromFile("graph_n_11.txt");

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

	return 0;
}
