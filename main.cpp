#include <iostream>
#include <windows.h>
#include <vector>
#include "./include/graphCreator.h"
#include "./include/algorithmsForTSP.h"

std::string readFile(std::string filename);
std::string generate(bool isDirected, int32_t verticies);
void help();

std::vector<std::vector<int32_t>> graph;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main(int argc, char **argv)
{
	std::string option;
	std::string sourceOfGraph;

	if (argc > 1)
		option = argv[1];
	else
	{
		SetConsoleTextAttribute(hConsole, 4);
		std::cout << "Error:";
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << " Wrong usage. Try ";
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "./main -h" << '\n';
		SetConsoleTextAttribute(hConsole, 15);
		return -1;
	}

	if (argc == 2 && option == "-h")
	{
		help();
		return 0;
	}
	else if (argc == 3 && option == "-f")
	{
		sourceOfGraph = readFile(argv[2]);

		if (sourceOfGraph == "error")
		{
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "Error:";
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << " Some problems with provided file" << '\n';
			return -1;
		}
	}
	else if (argc == 4 && option == "-g")
	{
		sourceOfGraph = generate(atoi(argv[2]), atoi(argv[3]));

		if (sourceOfGraph == "error")
		{
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "Error:";
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << " Wrong parameters passed" << '\n';
			return -1;
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 4);
		std::cout << "Error:";
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << " Wrong usage. Try ";
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "./main -h" << '\n';
		SetConsoleTextAttribute(hConsole, 15);
		return -1;
	}

	algorithmsForTSP algorithmsForTSP;

	std::cout << '\n' << "---Algorithms for solving TSP---" << '\n';


	std::cout << '\n';

	std::cout << sourceOfGraph << " graph:" << '\n';

	SetConsoleTextAttribute(hConsole, 3);

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

	std::pair<int32_t, std::vector<int32_t>> bruteForce = algorithmsForTSP.bruteForce(graph);
	std::pair<int32_t, std::vector<int32_t>> nearestNeighbour = algorithmsForTSP.nearestNeighbour(graph);
	std::pair<int32_t, std::vector<int32_t>> randomRoad = algorithmsForTSP.randomRoad(graph);

	SetConsoleTextAttribute(hConsole, 15);

	std::cout << '\n';

	std::cout << "---Brute force---" << '\n';
	std::cout << "Path: ";
	SetConsoleTextAttribute(hConsole, 14);
	for (int32_t v : bruteForce.second)
		std::cout << v << '-';
	std::cout << bruteForce.second[0] << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Cost: ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << bruteForce.first << '\n';
	SetConsoleTextAttribute(hConsole, 15);

	std::cout << '\n';

	std::cout << "---Nearest neighbour---" << '\n';
	std::cout << "Path: ";
	SetConsoleTextAttribute(hConsole, 14);
	for (int32_t v : nearestNeighbour.second)
		std::cout << v << '-';
	std::cout << nearestNeighbour.second[0] << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Cost: ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << nearestNeighbour.first << '\n';
	SetConsoleTextAttribute(hConsole, 15);

	std::cout << '\n';

	std::cout << "---Random path---" << '\n';
	std::cout << "Path: ";
	SetConsoleTextAttribute(hConsole, 14);
	for (int32_t v : randomRoad.second)
		std::cout << v << '-';
	std::cout << randomRoad.second[0] << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Cost: ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << randomRoad.first << '\n';
	SetConsoleTextAttribute(hConsole, 15);

	std::cout << '\n';

	return 0;
}

std::string readFile(std::string filename)
{
	graphCreator graphCreator;
	graph = graphCreator.readGraphFromFile(filename);
	if (graph.empty())
		return "error";
	else
		return "Loaded";
}

std::string generate(bool isDirected, int32_t verticies)
{
	if (verticies <= 0)
		return "error";

	graphCreator graphCreator;
	graph = graphCreator.generateRandomGraph(verticies, isDirected);

	return "Generated";
}

void help()
{
	std::cout << "---Algorithms for solving TSP--- (Usage)" << '\n';
	std::cout << '\n';
	std::cout << "Flags:" << '\n';
	std::cout << "-g ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "[isDirected][Number of Verticies]";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ": Generate graph following given parameters" << '\n';
	std::cout << '\n'
			  << "\tParameters:" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\t[isDirected]\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\t:\tDefinition if graph would be directed (1) or undirected(0)" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\t[Number of Verticies]";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\t:\tDefinition of number of verticies (Can accept only positive number)" << '\n';
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << "\n\t!!! WARNING !!!\n";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\tProgram will consider every string not being '0' as parameter for directed graph" << '\n';
	std::cout << '\n'
			  << "\tExamples:" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "\t./main -g 1 7\t\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ":\tGenerate directed graph with 7 verticies" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "\t./main -g 0 52\t\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ":\tGenerate undirected graph with 52 verticies" << '\n';
	std::cout << '\n';
	std::cout << "-f ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "[filename]";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ": Load graph from file" << '\n';
	std::cout << '\n'
			  << "\tParameteres:" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\t[filename]\t\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ":\tName of file with number of verticies and adjacency matrix of studied graph" << '\n';
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << "\n\t!!! WARNINGS !!!\n\t1.";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " Program can only process files with .txt extention" << '\n';
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << "\t2.";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " File should follow next structure (numbers can be separated by space or tab):" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\t\tN" << '\n';
	std::cout << "\t\t-1\t\tE_01\t\tE_02\t\t.  .  .\t\tE_0(N-1)" << '\n';
	std::cout << "\t\tE_10\t\t-1\t\tE_12\t\t.  .  .\t\tE_1(N-1)" << '\n';
	std::cout << "\t\tE_20\t\tE_21\t\t-1\t\t.  .  .\t\tE_2(N-1)" << '\n';
	std::cout << "\t\t.\t\t.\t\t.\t\t.\t\t." << '\n';
	std::cout << "\t\t.\t\t.\t\t.\t\t     .\t\t." << '\n';
	std::cout << "\t\t.\t\t.\t\t.\t\t          .\t." << '\n';
	std::cout << "\t\tE_(N-1)0\tE_(N-1)1\tE_(N-1)2\t.  .  .\t\t-1" << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\tWhere:" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\tN ";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "- number of verticies" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\tE_ij (i = 0..N-1, j = 0..N-1)";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " - positive weight of edge that goes from i to j" << '\n';
	SetConsoleTextAttribute(hConsole, 1);
	std::cout << "\n\tNotice:\n";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\tAssumed, that graph hasn't got any loops (edges that begins and ends in samy vertex)" << '\n';
	std::cout << "\tTherefore weights in matrix that may represent such loops (when i == j) are taken as -1" << '\n';
	std::cout << '\n'
			  << "\tExamples:" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "\t./main -f graph1.txt\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ":\tLoad graph from file 'graph1.txt'" << '\n';
	std::cout << '\n';
	std::cout << "-h : Show help information" << '\n';
	std::cout << '\n'
			  << "\tExamples:" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "\t./main -h\t\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ":\tShow help information" << '\n';
}
