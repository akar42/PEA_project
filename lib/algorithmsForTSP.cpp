#include "../include/algorithmsForTSP.h"

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

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::ts_with_time_limit(std::vector<std::vector<int32_t>> &graph, int neighbour_strategy, int time_limit_seconds, double &execution_time, bool logs)
{
	auto current_result = std::make_shared<std::pair<int32_t, std::vector<int32_t>>>();
	*current_result = {std::numeric_limits<int32_t>::max(), {}}; 

	std::atomic<bool> stop_flag(false);

	std::promise<void> promise;
	std::future<void> future = promise.get_future();

	std::thread algorithm_thread([&]()
		{
			try
			{
				tabu_search(graph, neighbour_strategy, current_result, stop_flag, execution_time, logs);
				promise.set_value(); 
			} 
			catch (...) 
			{
				promise.set_exception(std::current_exception());
			} 
		});

	if (future.wait_for(std::chrono::seconds(time_limit_seconds)) == std::future_status::timeout)
	{
		stop_flag = true; 
		if (logs)
		{
			std::cout << "Time limit exceeded. Returning the best known result.\n";
		}
		execution_time = time_limit_seconds;

		if (algorithm_thread.joinable())
			algorithm_thread.join(); 

		return *current_result; 
	}

	
	algorithm_thread.join();
	return *current_result;
}

void algorithmsForTSP::tabu_search(std::vector<std::vector<int32_t>> &graph, int neighbour_strategy, std::shared_ptr<std::pair<int32_t, std::vector<int32_t>>> current_result, std::atomic<bool> &stop_flag, double &execution_time, bool logs)
{
	auto start_time = std::chrono::high_resolution_clock::now();

	std::pair<int32_t, std::vector<int32_t>> first = greedyEstimation(graph);

	if (logs)
	{
		std::cout << "Greedy estimation: " << first.first << '\n';

		for (int32_t el : first.second)
		{
			std::cout << el << '-';
		}
		std::cout << first.second[0];
		std::cout << '\n';
	}


	std::pair<int32_t, std::vector<int32_t>> current = first;
	std::pair<int32_t, std::vector<int32_t>> best_known = first;

	std::vector<std::vector<int32_t>> tabu_list;

	int16_t critical_event = 0;

	int32_t max_iter = 1;
	while (graph.size() / max_iter != 0)
		max_iter *= 10;
	max_iter *= 10;

	for (int i = 0; i < max_iter; i++)
	{
		if (stop_flag) 
		{
			if (current.first < best_known.first)
			{
				best_known = current;
			}
			*current_result = best_known;
			return;
		}
		std::pair<int32_t, std::vector<int32_t>> neighbour = choose_neighbour(graph, current, tabu_list, neighbour_strategy, best_known.first);

		if (neighbour.second.empty())
		{
			critical_event++;
		}
		else
		{
			current = neighbour;
		}

		if (current.first < best_known.first)
		{
			best_known = current;
		}
		else if (current.first == best_known.first)
		{
			critical_event++;
		}

		for (auto it = tabu_list.begin(); it != tabu_list.end();)
		{
			(*it)[2] -= 1; 
			if ((*it)[2] == 0)
			{
				it = tabu_list.erase(it); 
			}
			else
			{
				it++; 
			}
		}

		if (critical_event == graph.size() / 4)
		{
			critical_event = 0;
			current = greedyEstimation(graph);
			while (current == first)
				current = greedyEstimation(graph);

			if (current.first < best_known.first)
			{
				best_known = current;
			}
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();					   
	execution_time = std::chrono::duration<double>(end_time - start_time).count(); 
	*current_result = best_known;
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::choose_neighbour(std::vector<std::vector<int32_t>> &graph,
																			std::pair<int32_t, std::vector<int32_t>> &current,
																			std::vector<std::vector<int32_t>> &tabu_list,
																			int32_t &neighbour_strategy,
																			int32_t best_cost)
{
	randomEngine.seed(time(NULL));
	std::uniform_int_distribution<uint32_t> route_index_dist(0, current.second.size() - 1);

	int best_diff = std::numeric_limits<int32_t>::min();
	std::pair<int32_t, std::vector<int32_t>> best_route;
	int32_t best_left = 0;
	int32_t best_right = 0;

	std::pair<int32_t, std::vector<int32_t>> neighbour;

	for (int i = 0; i < current.second.size() * current.second.size(); i++)
	{
		int32_t left = route_index_dist(randomEngine);
		int32_t right = route_index_dist(randomEngine);

		while (left >= right)
		{
			left = route_index_dist(randomEngine);
			right = route_index_dist(randomEngine);
		}

		bool in_tabu_list = false;
		for (int j = 0; j < tabu_list.size(); j++)
		{
			int32_t tabu_left = tabu_list[j][0];
			int32_t tabu_right = tabu_list[j][1];

			if (left == tabu_left && right == tabu_right)
			{
				in_tabu_list = true;
				break;
			}
		}

		std::pair<int32_t, std::vector<int32_t>> candidate;
		switch (neighbour_strategy)
		{
		case 1:
			candidate = calculateNeighbour_Swap(graph, current.second, left, right);
			break;
		case 2:
			candidate = calculateNeighbour_2Opt(graph, current.second, left, right);
			break;
		case 3:
			candidate = calculateNeighbour_Insert(graph, current.second, left, right);
			break;
		default:
			break;
		}

		if (in_tabu_list)
		{
			// Aspiration
			if (candidate.first >= best_cost) continue;
		}

		neighbour = candidate;

		int32_t diff = current.first - neighbour.first;

		if (diff > best_diff && diff > -current.first/5)
		{
			best_diff = diff;
			best_route = neighbour;
			best_left = left;
			best_right = right;
		}
	}

	if (best_diff != std::numeric_limits<int32_t>::min())
	{
		int32_t cadence = best_route.second.size() + 1;
		std::vector<int32_t> el;
		if (neighbour_strategy == 3)
		{
			if (best_left < best_right) 
			{
				el = {best_right, best_left + 1, cadence};
			}
			else 
			{
				el = {best_right - 1, best_left, cadence};
			}
		}
		else
		{
			el = {best_left, best_right, cadence};
		}
		tabu_list.push_back(el);
	}

	return best_route;
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::greedyEstimation(std::vector<std::vector<int32_t>> &graph)
{
	randomEngine.seed(time(NULL));
	std::uniform_int_distribution<uint32_t> route_index_dist(0, graph.size() - 1);

	int32_t start_index = route_index_dist(randomEngine);

	int32_t cost = 0;
	std::vector<int32_t> route;
	route.push_back(start_index);

	std::vector<bool> visited(graph.size(), false);

	visited[start_index] = true;

	for (int32_t i = start_index; route.size() != graph.size(); i = route.back())
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

	cost += graph[route.back()][start_index];

	return {cost, route};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::sa_with_time_limit(
	std::vector<std::vector<int32_t>> &graph,
	double alpha,
	int time_limit_seconds,
	double &execution_time,
	bool logs)
{
	auto current_result = std::make_shared<std::pair<int32_t, std::vector<int32_t>>>();
	*current_result = {std::numeric_limits<int32_t>::max(), {}}; 

	std::atomic<bool> stop_flag(false);

	std::promise<void> promise;
	std::future<void> future = promise.get_future();

	std::thread algorithm_thread([&]()
		{
			try 
			{
				simulated_annealing(graph, alpha, current_result, stop_flag, execution_time, logs);
				promise.set_value(); 
			} 
			catch (...) 
			{
				promise.set_exception(std::current_exception());
			} 
		});

	
	if (future.wait_for(std::chrono::seconds(time_limit_seconds)) == std::future_status::timeout)
	{
		stop_flag = true; 
		if (logs)
		{
			std::cout << "Time limit exceeded. Returning the best known result.\n";
		}
		execution_time = time_limit_seconds;

		if (algorithm_thread.joinable())
			algorithm_thread.join(); 

		return *current_result; 
	}

	algorithm_thread.join();
	return *current_result;
}

void algorithmsForTSP::simulated_annealing(std::vector<std::vector<int32_t>> &graph, double alpha, std::shared_ptr<std::pair<int32_t, std::vector<int32_t>>> current_result, std::atomic<bool> &stop_flag, double &execution_time, bool logs)
{
	auto start_time = std::chrono::high_resolution_clock::now();

	std::pair<int32_t, std::vector<int32_t>> current = nearestNeighbour(graph);

	std::pair<int32_t, std::vector<int32_t>> best_known = current;

	if (logs)
	{
		std::cout << "Greedy estimation: " << current.first << '\n';

		for (int32_t el : current.second)
		{
			std::cout << el << '-';
		}
		std::cout << current.second[0];
		std::cout << '\n';
	}


	int32_t cost_sum = 0;
	for (std::vector<int32_t> row : graph)
	{
		for (int32_t el : row)
		{
			if (el != -1)
				cost_sum += el;
		}
	}

	double temperature = (double) 100 * cost_sum / (graph.size() * (graph.size() - 1));
	// double temperature = 10.0;
	// double temperature = (double) 0.25 * current.first / graph.size();

	// while (graph.size() / (int) temperature != 0) temperature *= 10;

	if (logs)
	{
		std::cout << "Start temperature: " << temperature << '\n';
	}

	randomEngine.seed(time(NULL));
	std::uniform_int_distribution<uint32_t> route_index_dist(0, current.second.size() - 1);
	std::uniform_real_distribution<> rand_prop_dist(0, 1);

	while (temperature > 0.000000001)
	{
		for (int i = 0; i < graph.size() * 10; i++)
		{
			if (stop_flag) 
			{
				if (current.first < best_known.first)
				{
					best_known = current;
				}
				*current_result = best_known;
				if (logs)
				{
					std::cout << "Current temperature: " << temperature << '\n';
				}
				return;
			}

			int32_t city1 = route_index_dist(randomEngine);
			int32_t city2 = route_index_dist(randomEngine);

			while (city1 == city2)
			{
				city1 = route_index_dist(randomEngine);
				city2 = route_index_dist(randomEngine);
			}

			std::pair<int32_t, std::vector<int32_t>> candidate = calculateNeighbour_Swap(graph, current.second, city1, city2);

			int32_t delta = candidate.first - current.first;
			double probability_of_change = exp(-1.0 * delta / temperature);
			double probability = rand_prop_dist(randomEngine);

			if (delta <= 0 || (probability < probability_of_change))
			{
				current = candidate;

				if (candidate.first < best_known.first)
				{
					best_known = candidate;
				}
			}
		}

		temperature *= alpha;
	}

	if (logs)
	{
		std::cout << "End temperature: " << temperature << '\n';
	}

	auto end_time = std::chrono::high_resolution_clock::now();					   
	execution_time = std::chrono::duration<double>(end_time - start_time).count(); 
	*current_result = best_known;
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::calculateNeighbour_2Opt(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> &route, int32_t city1, int32_t city2)
{
	std::vector<int32_t> new_route(route.size(), 0);

	for (int i = 0; i < route.size(); i++)
	{
		if (i > city1 && i <= city2)
		{
			// Backwards
			new_route[i] = route[city2 + city1 + 1 - i];
		}
		else
		{
			new_route[i] = route[i];
		}
	}

	int32_t cost = 0;

	for (int i = 0; i < new_route.size() - 1; i++)
	{
		cost += graph[new_route[i]][new_route[i + 1]];
	}

	cost += graph[new_route[new_route.size() - 1]][new_route[0]];

	return {cost, new_route};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::calculateNeighbour_Swap(std::vector<std::vector<int32_t>> &graph, std::vector<int32_t> route, int32_t city1, int32_t city2)
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