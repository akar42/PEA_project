#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <chrono>
#include "./include/graphCreator.h"
#include "./include/algorithmsForTSP.h"

std::string readFile(std::string filename);
std::string generate(bool isDirected, int32_t verticies);
void help();
std::string startTest(std::string filename);

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
	else if (argc == 3 && option == "-t")
	{
		sourceOfGraph = startTest(argv[2]);

		if (sourceOfGraph == "error_in")
		{
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "Error:";
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << " Some problems with provided file" << '\n';
			return -1;
		}
		else if (sourceOfGraph == "error_out")
		{
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "Error:";
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << " Can't open file output.csv" << '\n';
			return -1;
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Finished" << '\n';
			SetConsoleTextAttribute(hConsole, 15);
			return 0;
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

	bool isDirected = false;
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = i + 1; j < graph.size(); j++)
		{
			if (graph[i][j] != graph[j][i])
			{
				isDirected = true;
				break;
			}
		}
		if (isDirected) break;
	}

	std::pair<int32_t, std::vector<int32_t>> bfs = algorithmsForTSP.bfs(graph, isDirected);
	std::pair<int32_t, std::vector<int32_t>> dfs = algorithmsForTSP.dfs(graph, isDirected);
	std::pair<int32_t, std::vector<int32_t>> best_first_search = algorithmsForTSP.best_first_search(graph, isDirected);

	SetConsoleTextAttribute(hConsole, 15);

	std::cout << '\n';

	std::cout << "---BFS---" << '\n';
	std::cout << "Path: ";
	SetConsoleTextAttribute(hConsole, 14);
	for (int32_t v : bfs.second)
		std::cout << v << '-';
	std::cout << bfs.second[0] << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Cost: ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << bfs.first << '\n';
	SetConsoleTextAttribute(hConsole, 15);

	std::cout << '\n';

	std::cout << "---DFS---" << '\n';
	std::cout << "Path: ";
	SetConsoleTextAttribute(hConsole, 14);
	for (int32_t v : dfs.second)
		std::cout << v << '-';
	std::cout << dfs.second[0] << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Cost: ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << dfs.first << '\n';
	SetConsoleTextAttribute(hConsole, 15);

	std::cout << '\n';

	std::cout << "---Best First Search---" << '\n';
	std::cout << "Path: ";
	SetConsoleTextAttribute(hConsole, 14);
	for (int32_t v : best_first_search.second)
		std::cout << v << '-';
	std::cout << best_first_search.second[0] << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Cost: ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << best_first_search.first << '\n';
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

std::string startTest(std::string filename)
{
	std::ifstream fin;

	fin.open(filename);

	if (!fin.is_open())
	{
		return "error_in";
	}

	int32_t iterations_num = 0;
	fin >> iterations_num;

	int sizes_length = 0;
	fin >> sizes_length;


	std::vector<int32_t> sizes;

	for (int i = 0; i < sizes_length; i++)
	{
		int32_t el;
		fin >> el;
		sizes.push_back(el);
	}

	fin.close();

	graphCreator graphCreator;
	algorithmsForTSP algorithmsForTSP;

	std::vector<std::vector<int32_t>> graph;

	std::ofstream fout;

	fout.open("output.csv");

	if (!fout.is_open())
	{
		// std::cout << "Can't open file" << '\n';
		return "error_out";
	}

	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "Algorithms for TSP" << '\n' << "Test started successfully" << '\n';
	SetConsoleTextAttribute(hConsole, 15);

	fout << ",Undirected graph,,, Directed graph" << '\n';
	fout << "Number of Verticies, BFS [ms], DFS [ms], Best First Search [ms], BFS [ms], DFS [ms], Best First Search [ms],\n";

	for (int32_t n: sizes)
	{
		for (int i = 0; i < iterations_num; ++i)
		{
			fout << n << ',';
			// -------------Undirected----------------------
			bool isDirected = false;
			graph = graphCreator.generateRandomGraph(n, isDirected);

			std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
			algorithmsForTSP.bfs(graph, isDirected);
			std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double, std::milli> delta = end - start;
			fout << delta.count() << ',';

			start = std::chrono::high_resolution_clock::now();
			algorithmsForTSP.dfs(graph, isDirected);
			end = std::chrono::high_resolution_clock::now();

			delta = end - start;
			fout << delta.count() << ',';

			start = std::chrono::high_resolution_clock::now();
			algorithmsForTSP.best_first_search(graph, isDirected);
			end = std::chrono::high_resolution_clock::now();

			delta = end - start;
			fout << delta.count() << ',';


			isDirected = !isDirected;
			
			// --------------Directed-----------------------
			graph = graphCreator.generateRandomGraph(n, isDirected);

			start = std::chrono::high_resolution_clock::now();
			algorithmsForTSP.bfs(graph, isDirected);
			end = std::chrono::high_resolution_clock::now();

			delta = end - start;
			fout << delta.count() << ',';

			start = std::chrono::high_resolution_clock::now();
			algorithmsForTSP.dfs(graph, isDirected);
			end = std::chrono::high_resolution_clock::now();

			delta = end - start;
			fout << delta.count() << ',';

			start = std::chrono::high_resolution_clock::now();
			algorithmsForTSP.best_first_search(graph, isDirected);
			end = std::chrono::high_resolution_clock::now();
			
			delta = end - start;
			fout << delta.count() << '\n';
		}
	}
	fout.close();
	return "ok";
}

void help()
{
	std::cout << '\n';
	std::cout << "\t---Algorithms for solving TSP--- (Usage)" << '\n';
	std::cout << '\n';
	std::cout << "Flags:" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
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
	SetConsoleTextAttribute(hConsole, 14);
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
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "-t ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "[filename_with_test_parameters]";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ": Start time execution test for all algorithms by parameteres specified in file" << '\n';
	std::cout << '\n';
	std::cout << "\tParameters:" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\t[filename_with_test_parameters]\t\t:\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Name of file with parameters for time execution test" << '\n';
	std::cout << '\n';
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << "\t!!! WARNING !!!\n\t1.";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " Program can only process files with .txt extention" << '\n';
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << "\t2.";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " File should follow next structure (numbers can be separated by space or tab):" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\t\tN" << '\n';
	std::cout << "\t\tk" << '\n';
	std::cout << "\t\tn_1 n_2 .  .  . n_k" << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\tWhere:" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\tN";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " - number of iterations" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\tk";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " - number of graph sizes (number of verticies)" << '\n';
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "\tn_i (i = 0..k)";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " - number of verticies" << '\n';
	SetConsoleTextAttribute(hConsole, 1);
	std::cout << "\n\tNotice:\n";
	std::cout << "\t1.";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " All collected data will be saved to file 'output.csv'" << '\n';
	SetConsoleTextAttribute(hConsole, 1);
	std::cout << "\t2.";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " Test will be conducted be the following algorithm (pseudocode):" << "\n\n";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\tfor i in ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "0..k" << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\t\tfor j in ";
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "1..N" << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\t\t\t1. Generate undirected graph with "; 
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "n_i";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " verticies" << '\n';
	std::cout << "\t\t\t2. Measure execution time for all algorithm seperately and write them to 'output.csv'" << '\n';
	std::cout << "\t\t\t3. Generate directed graph with "; 
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << "n_i";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " verticies" << '\n';
	std::cout << "\t\t\t4. Measure execution time for all algorithm seperately and write them to 'output.csv'" << '\n';
	std::cout << "\t\tend for" << '\n';
	std::cout << "\tend for" << '\n';
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "\n\tExamples:" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "\t./main -t params_1.txt\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout <<":\tStart time execution test for all algorithms by parameteres" << '\n';
	std::cout << "\t\t\t\t\tspecified in file 'params_1.txt'" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "-h";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << " : Show help information" << '\n';
	std::cout << '\n'
			  << "\tExamples:" << '\n';
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "\t./main -h\t\t";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ":\tShow help information" << '\n';
}
