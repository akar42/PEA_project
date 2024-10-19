#include "../include/graphCreator.h"

std::vector<std::vector<int32_t>> graphCreator::generateRandomGraph(int32_t verticies, bool isDirected)
{
	randomEngine.seed(time(NULL));
	std::uniform_int_distribution<uint32_t> uint_dist_1_100(1, 100);

	std::vector<std::vector<int32_t>> result(verticies, std::vector<int32_t>(verticies, -1));

	if (isDirected)
	{
		for (int i = 0; i < result.size(); ++i)
		{
			for (int j = 0; j < result[i].size(); ++j)
			{
				if (i == j)
					continue;
				result[i][j] = uint_dist_1_100(randomEngine);
			}
		}
	}
	else
	{
		for (int i = 0; i < result.size() - 1; ++i)
		{
			for (int j = i + 1; j < result.size(); ++j)
			{
				int32_t weight = uint_dist_1_100(randomEngine);
				result[i][j] = weight;
				result[j][i] = weight;
			}
		}
	}

	return result;
}

std::vector<std::vector<int32_t>> graphCreator::readGraphFromFile(std::string filename)
{
	std::vector<std::vector<int32_t>> result;

	std::ifstream fin;
	fin.open(filename);

	if (fin.is_open())
	{
		uint32_t verticies;

		fin >> verticies;

		for (int i = 0; i < verticies; ++i)
		{
			result.push_back(std::vector<int32_t>(verticies));
			for (int j = 0; j < verticies; ++j)
			{
				fin >> result[i][j];
			}
		}

		fin >> this->min_sum;

		fin.close();
	}

	return result;
}