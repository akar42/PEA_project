#include "../include/algorithmsForTSP.h"

algorithmsForTSP::algorithmsForTSP()
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	randomEngine.seed(static_cast<unsigned>(seed));
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::nnRouteFromVertex(std::vector<std::vector<int32_t>> &graph, int32_t start_vertex)
{
	int32_t cost = 0;
	std::vector<int32_t> route;
	route.push_back(start_vertex);

	std::vector<bool> visited(graph.size(), false);

	visited[start_vertex] = true;

	for (int32_t i = start_vertex; route.size() != graph.size(); i = route.back())
	{
		int32_t min_neighb = -1;
		int32_t min_neighb_cost = std::numeric_limits<int32_t>::max();

		for (int j = 0; j < graph.size(); j++)
		{
			if (visited[j])
				continue;

			if (graph[i][j] < min_neighb_cost)
			{
				min_neighb = j;
				min_neighb_cost = graph[i][j];
			}
		}

		route.push_back(min_neighb);
		visited[min_neighb] = true;
		cost += min_neighb_cost;
	}

	cost += graph[route.back()][start_vertex];

	return {cost, route};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::randomRoute(std::vector<std::vector<int32_t>> &graph)
{
	std::vector<bool> visited(graph.size(), false);

	std::vector<int32_t> route;
	int cost = 0;

	std::uniform_int_distribution<uint32_t> route_index_dist(0, graph.size() - 1);

	while (route.size() != graph.size())
	{
		int32_t v = route_index_dist(randomEngine);
		if (!visited[v])
		{
			route.push_back(v);
			visited[v] = true;
		}
	}

	for (int i = 0; i < route.size() - 1; ++i)
	{
		cost += graph[route[i]][route[i + 1]];
	}

	cost += graph[route.back()][route[0]];

	return {cost, route};
}

std::vector<std::pair<int32_t, std::vector<int32_t>>> algorithmsForTSP::initializeColony(std::vector<std::vector<int32_t>> &graph, int32_t n)
{
	std::uniform_int_distribution<uint32_t> route_index_dist(0, graph.size() - 1);
	std::vector<bool> already_picked(graph.size(), false);

	std::vector<std::pair<int32_t, std::vector<int32_t>>> colony;
	std::set<std::vector<int32_t>> unique_routes;

	int32_t nn_amount = 0;
	int32_t rand_amount = 0;

	for (int i = 0; i < n && i < graph.size(); i++)
	{
		int32_t start_vertex = route_index_dist(randomEngine);

		while (already_picked[start_vertex]) start_vertex = route_index_dist(randomEngine);
		already_picked[start_vertex] = true;

		std::pair<int32_t, std::vector<int32_t>> nn_route = nnRouteFromVertex(graph, start_vertex);

		// Ensuring that route is unique
		std::vector<int32_t> route = nn_route.second;

		auto min_it = std::min_element(route.begin(), route.end());
		std::rotate(route.begin(), min_it, route.end());

		std::vector<int32_t> reverse_route(route.rbegin(), route.rend());
		if (reverse_route < route)
			route = reverse_route;

		if (unique_routes.insert(route).second)
		{
			colony.push_back(nn_route);
			nn_amount++;
		}

	}

	// If not enough NN routes - adding random routes to colony
	while (colony.size() < n)
	{
		std::pair<int32_t, std::vector<int32_t>> rand_route = randomRoute(graph);

		// Ensuring that route is unique
		std::vector<int32_t> route = rand_route.second;

		auto min_it = std::min_element(route.begin(), route.end());
		std::rotate(route.begin(), min_it, route.end());

		std::vector<int32_t> reverse_route(route.rbegin(), route.rend());
		if (reverse_route < route)
			route = reverse_route;

		if (unique_routes.insert(route).second)
		{
			colony.push_back(rand_route);
			rand_amount++;
		}

		// std::cout << colony.size() << '\n';
	}

	// std::cout << "Percentage of NN routes: " << 100.0 * nn_amount / n << "%" << '\n';

	return colony;
}

std::pair<std::vector<int32_t>, std::vector<int32_t>> algorithmsForTSP::pmx_crossover(std::vector<int32_t> &parent1, std::vector<int32_t> &parent2)
{
	int32_t n = parent1.size();

    std::uniform_int_distribution<uint32_t> dist(0, n - 1);

    int32_t start = dist(randomEngine);
    int32_t end = dist(randomEngine);

    // Ensure point1 < point2
    if (start > end) std::swap(start, end);

    std::vector<int32_t> offspring1(n, -1);
    std::vector<int32_t> offspring2(n, -1);

    for (size_t i = start; i <= end; ++i) {
        offspring1[i] = parent2[i];
        offspring2[i] = parent1[i];
    }

    std::unordered_map<int, int> mapping1, mapping2;
    for (size_t i = start; i <= end; ++i) {
        mapping1[parent2[i]] = parent1[i];
        mapping2[parent1[i]] = parent2[i];
    }

    for (size_t i = 0; i < n; ++i) {
        if (i >= start && i <= end) continue;

        int value = parent1[i];
        while (mapping1.find(value) != mapping1.end()) {
            value = mapping1[value];
        }
        offspring1[i] = value;
    }

    for (size_t i = 0; i < n; ++i) {
        if (i >= start && i <= end) continue;

        int value = parent2[i];
        while (mapping2.find(value) != mapping2.end()) {
            value = mapping2[value];
        }
        offspring2[i] = value;
    }

    return {offspring1, offspring2};
}

std::pair<std::vector<int32_t>, std::vector<int32_t>> algorithmsForTSP::ox_crossover(std::vector<int32_t> &parent1, std::vector<int32_t> &parent2)
{
	// Ensure both parents have the same size
    if (parent1.size() != parent2.size()) {
        throw std::invalid_argument("Parents must have the same size.");
    }

    size_t n = parent1.size();

    // Define two crossover points
    size_t start = 3; // Example crossover point 1
    size_t end = 5;   // Example crossover point 2

    // Initialize offspring
    std::vector<int> offspring1(n, -1);
    std::vector<int> offspring2(n, -1);

    // Step 1: Copy the crossover segment from parents to offspring
    for (size_t i = start; i <= end; ++i) {
        offspring1[i] = parent2[i];
        offspring2[i] = parent1[i];
    }

    // Step 2: Fill the remaining positions in offspring
    auto fillOffspring = [&](const std::vector<int> &parent, std::vector<int> &offspring) {
        size_t current_index = (end + 1) % n;
        for (size_t i = 0; i < n; ++i) {
            size_t parent_index = (end + 1 + i) % n;
            int gene = parent[parent_index];

            // Check if the gene is already in the crossover segment
            if (std::find(offspring.begin(), offspring.end(), gene) == offspring.end()) {
                offspring[current_index] = gene;
                current_index = (current_index + 1) % n;
            }
        }
    };

    fillOffspring(parent1, offspring1);
    fillOffspring(parent2, offspring2);

    return {offspring1, offspring2};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::calculateNeighbour_Swap(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &route, int32_t city1, int32_t city2)
{
	std::vector<int32_t> new_route(route.begin(), route.end());
	std::swap(new_route[city1], new_route[city2]);

	int32_t cost = 0;

	for (int i = 0; i < new_route.size() - 1; i++)
	{
		cost += graph[new_route[i]][new_route[i + 1]];
	}

	cost += graph[new_route[new_route.size() - 1]][new_route[0]];

	return {cost, new_route};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::calculateNeighbour_Insert(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &route, int32_t city1, int32_t city2)
{
	std::vector<int32_t> new_route(route.begin(), route.end());

    int32_t numberToInsert = new_route[city2];

    if (city2 < city1) {
        city1--;
    }

    new_route.erase(new_route.begin() + city2);

    new_route.insert(new_route.begin() + city1 + 1, numberToInsert);

	int32_t cost = 0;

	for (int i = 0; i < new_route.size() - 1; i++)
	{
		cost += graph[new_route[i]][new_route[i + 1]];
	}

	cost += graph[new_route[new_route.size() - 1]][new_route[0]];

	return {cost, new_route};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::genetic_algorithm(std::vector<std::vector<int32_t>> &graph, int32_t colony_size, int32_t crossing_strategy, int32_t mutation_stategy, float crossover_rate, float mutation_rate, float time_limit)
{
	auto start_time = std::chrono::high_resolution_clock::now();
	std::vector<std::pair<int32_t, std::vector<int32_t>>> colony = initializeColony(graph, colony_size);

	int32_t graph_size = graph.size();
	std::pair<int32_t, std::vector<int32_t>> best_result = {std::numeric_limits<int32_t>::max(), std::vector<int32_t>()};

	// for (auto &pair: colony)
	// {
	// 	if (pair.first < best_result.first)
	// 	{
	// 		best_result = pair;
	// 	}
	// }

	// std::cout << best_result.first << '\n';

	std::uniform_real_distribution<> rand_prop_dist(0, 1);
	std::uniform_int_distribution<uint32_t> route_index_dist(0, graph.size() - 1);
	std::uniform_int_distribution<uint32_t> population_index_dist(0, colony_size / 10 - 1);

	while (true)
	{
		auto time_point = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration<double>(time_point - start_time).count() >= time_limit)
		{
			for (auto &pair: colony)
			{
				if (pair.first < best_result.first)
				{
					best_result = pair;
				}
			}

			return best_result;
		}
		// Natural selection - only best 10% stays alive
		std::sort(colony.begin(), colony.end(), [](const auto &a, const auto &b)
				  { return a.first < b.first; });
		colony.resize(colony.size() / 10);

		// After "death" our best current solution - 1st chromosome in colony
		best_result = colony[0];

		std::shuffle(colony.begin(), colony.end(), randomEngine);

		// std::cout << "Generation:" << i + 1 << '\n'; 

		// Crossover
		int32_t old_size = colony.size();
		while (colony.size() < colony_size)
		{
			// std::cout << p1 << '-' << p2 << '\n';
			int32_t p1 = population_index_dist(randomEngine);
			int32_t p2 = population_index_dist(randomEngine);

			while (p1 == p2) p2 = population_index_dist(randomEngine);


			if (rand_prop_dist(randomEngine) <= crossover_rate)
			{
				std::pair<std::vector<int32_t>, std::vector<int32_t>> offsprings;
				if (crossing_strategy == 1)
				{
					offsprings = pmx_crossover(colony[p1].second, colony[p2].second);
				}
				else
				{
					offsprings = ox_crossover(colony[p1].second, colony[p2].second);
				}

				int32_t cost_o1 = 0;
				int32_t cost_o2 = 0;

				for (int k = 0; k < graph_size; k++)
				{
					cost_o1 += graph[offsprings.first[((k - 1) % graph_size + graph_size) % graph_size]][offsprings.first[k]];
					cost_o2 += graph[offsprings.second[((k - 1) % graph_size + graph_size) % graph_size]][offsprings.second[k]];
				}

				std::pair<int32_t, std::vector<int32_t>> o1 = {cost_o1, offsprings.first};
				std::pair<int32_t, std::vector<int32_t>> o2 = {cost_o2, offsprings.second};

				// Mutation
				if (rand_prop_dist(randomEngine) <= mutation_rate)
				{
					// std::cout << "gg" << '\n';
					int32_t right = route_index_dist(randomEngine);
					int32_t left = route_index_dist(randomEngine);

					// std::cout << right << '-' << left << '\n';

					while (right >= left)
					{
						right = route_index_dist(randomEngine);
						left = route_index_dist(randomEngine);
					}

					if (mutation_stategy == 1)
					{
						o1 = calculateNeighbour_Swap(graph, o1.second, right, left);
						o2 = calculateNeighbour_Swap(graph, o2.second, right, left);
					}
					else
					{
						o1 = calculateNeighbour_Insert(graph, o1.second, right, left);
						o2 = calculateNeighbour_Insert(graph, o2.second, right, left);
					}
				}

				colony.push_back(o1);
				colony.push_back(o2);
			}
		}

		// std::cout << "Amount after crossover: " << colony.size() << '\n';
	}

	// return best_result;
}
