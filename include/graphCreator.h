#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <time.h>

class graphCreator
{
private:
	int32_t min_sum = -1;
	std::mt19937 randomEngine;

public:
	std::vector<std::vector<int32_t>> generateRandomGraph(int32_t verticies, bool isDirected);
	std::vector<std::vector<int32_t>> readGraphFromFile(std::string filename);
};