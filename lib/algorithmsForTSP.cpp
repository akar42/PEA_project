#include "../include/algorithmsForTSP.h"

// Implementation of the Node class
Node::Node(int matrix_size)
    : current_vertex(-1),
      lower_bound(0),
      cost(0),
      visited_bits(0),
      current_path(nullptr),
      current_path_length(0),
      matrix_size(matrix_size) {
    // No need to allocate reduced_matrix
}

Node::Node(const Node& other)
    : current_vertex(other.current_vertex),
      lower_bound(other.lower_bound),
      cost(other.cost),
      visited_bits(other.visited_bits),
      current_path_length(other.current_path_length),
      matrix_size(other.matrix_size) {
    if (current_path_length > 0) {
        current_path = new int32_t[current_path_length];
        std::copy(other.current_path, other.current_path + current_path_length, current_path);
    } else {
        current_path = nullptr;
    }
}

// Assignment Operator Implementation
Node& Node::operator=(const Node& other) {
    if (this != &other) {
        // Free existing resources
        delete[] current_path;

        // Copy data
        current_vertex = other.current_vertex;
        lower_bound = other.lower_bound;
        cost = other.cost;
        visited_bits = other.visited_bits;
        current_path_length = other.current_path_length;
        matrix_size = other.matrix_size;

        // Deep copy current_path
        if (current_path_length > 0) {
            current_path = new int32_t[current_path_length];
            std::copy(other.current_path, other.current_path + current_path_length, current_path);
        } else {
            current_path = nullptr;
        }
    }
    return *this;
}

// Перемещающий конструктор
Node::Node(Node&& other) noexcept
    : current_vertex(other.current_vertex),
      lower_bound(other.lower_bound),
      cost(other.cost),
      visited_bits(other.visited_bits),
      current_path(other.current_path),
      current_path_length(other.current_path_length),
      matrix_size(other.matrix_size) {
    // Обнуляем указатели в объекте-источнике
    other.current_path = nullptr;
    other.current_path_length = 0;
}

// Перемещающий оператор присваивания
Node& Node::operator=(Node&& other) noexcept {
    if (this != &other) {
        // Освобождаем текущие ресурсы
        delete[] current_path;

        // Перемещаем данные
        current_vertex = other.current_vertex;
        lower_bound = other.lower_bound;
        cost = other.cost;
        visited_bits = other.visited_bits;
        current_path = other.current_path;
        current_path_length = other.current_path_length;
        matrix_size = other.matrix_size;

        // Обнуляем указатели в объекте-источнике
        other.current_path = nullptr;
        other.current_path_length = 0;
    }
    return *this;
}

// Overload operator< for priority_queue
bool Node::operator<(const Node& other) const {
    // Invert the comparison to create a min-heap based on lower_bound
    return this->lower_bound > other.lower_bound;
}

Node::~Node() {
	if (current_path != nullptr)
    	delete[] current_path;
}

int32_t algorithmsForTSP::calculateLowerBound(const std::vector<std::vector<int32_t>>& graph, const Node& node, int32_t start_vertex) {
    int32_t lowerBound = 0;
    int n = graph.size();

    int currentNode = node.current_vertex;

    // Add the minimum outgoing edge cost for the current node
    int32_t minOutgoing = std::numeric_limits<int32_t>::max();
    for (int j = 0; j < n; ++j) {
        if (!(node.visited_bits & (1 << j)) && graph[currentNode][j] != -1 && graph[currentNode][j] < minOutgoing) {
            minOutgoing = graph[currentNode][j];
        }
    }
    if (minOutgoing != std::numeric_limits<int32_t>::max())
        lowerBound += minOutgoing;

    // Add the minimum incoming edge cost to return to the start node
    int32_t minIncoming = std::numeric_limits<int32_t>::max();
    for (int i = 0; i < n; ++i) {
        if (!(node.visited_bits & (1 << i)) && graph[i][start_vertex] != -1 && graph[i][start_vertex] < minIncoming) {
            minIncoming = graph[i][start_vertex];
        }
    }
    if (minIncoming != std::numeric_limits<int32_t>::max())
        lowerBound += minIncoming;

    // Add the minimum outgoing and incoming edge costs for unvisited nodes
    for (int i = 0; i < n; ++i) {
        if (!(node.visited_bits & (1 << i))) {
            int32_t minOut = std::numeric_limits<int32_t>::max();
            int32_t minIn = std::numeric_limits<int32_t>::max();
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    if (graph[i][j] != -1 && graph[i][j] < minOut) {
                        minOut = graph[i][j];
                    }
                    if (graph[j][i] != -1 && graph[j][i] < minIn) {
                        minIn = graph[j][i];
                    }
                }
            }
            if (minOut != std::numeric_limits<int32_t>::max())
                lowerBound += minOut;
            if (minIn != std::numeric_limits<int32_t>::max())
                lowerBound += minIn;
        }
    }

    // Since each edge is counted twice, divide by 2
    return lowerBound / 2;
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::dfs(std::vector<std::vector<int32_t>> &graph, bool isDirected, int32_t &start_vertex)
{
    int n = graph.size();
    std::stack<Node, std::deque<Node>> dfs_stack;

    Node start_node(n);
    start_node.current_vertex = start_vertex;
    start_node.visited_bits = 1 << start_vertex;
    start_node.current_path_length = 1;
    start_node.current_path = new int32_t[1];
    start_node.current_path[0] = start_vertex;
    start_node.cost = 0;

    // Calculate the initial lower bound using the improved function
    start_node.lower_bound = start_node.cost + calculateLowerBound(graph, start_node, start_vertex);

    dfs_stack.push(start_node);

    int32_t upper_bound = std::numeric_limits<int32_t>::max();
    std::vector<int32_t> optimal_path;

    while (!dfs_stack.empty()) {
        Node current_node = dfs_stack.top();
        dfs_stack.pop();

        // Prune nodes whose lower bound exceeds the current best cost
        if (current_node.lower_bound >= upper_bound)
            continue;

        // If it's a leaf node
        if (current_node.current_path_length == n) {
            int32_t return_edge_cost = graph[current_node.current_vertex][start_vertex];
            if (return_edge_cost == -1)
                continue;

            int32_t total_cost = current_node.cost + return_edge_cost;

            if (total_cost < upper_bound) {
                upper_bound = total_cost;
                optimal_path.assign(current_node.current_path, current_node.current_path + current_node.current_path_length);
                // optimal_path.push_back(start_vertex); // Return to start
            }
            continue;
        }

        // Generate child nodes for all unvisited vertices
        for (int i = n - 1; i >= 0; i--) {
            uint32_t bit_mask = 1 << i;
            if (!(current_node.visited_bits & bit_mask)) {
                int32_t edge_cost = graph[current_node.current_vertex][i];
                if (edge_cost == -1)
                    continue;

                Node child(n);
                child.current_vertex = i;
                child.visited_bits = current_node.visited_bits | bit_mask;
                child.current_path_length = current_node.current_path_length + 1;

                // Allocate and copy current_path
                child.current_path = new int32_t[child.current_path_length];
                std::copy(current_node.current_path, current_node.current_path + current_node.current_path_length, child.current_path);
                child.current_path[child.current_path_length - 1] = i;

                // Update cost and calculate lower_bound using the improved function
                child.cost = current_node.cost + edge_cost;
                child.lower_bound = child.cost + calculateLowerBound(graph, child, start_vertex);

                // Prune nodes whose lower bound exceeds the current best cost
                if (child.lower_bound >= upper_bound)
                    continue;

                dfs_stack.emplace(std::move(child));
            }
        }
    }

    return {upper_bound, optimal_path};
}

// BFS function for TSP using Branch and Bound
std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::bfs(std::vector<std::vector<int32_t>> &graph, bool isDirected, int32_t &start_vertex) {
    int n = graph.size();
    std::queue<Node, std::deque<Node>> bfs_queue;

    Node start_node(n);
    start_node.current_vertex = start_vertex;
    start_node.visited_bits = 1 << start_vertex;
    start_node.current_path_length = 1;
    start_node.current_path = new int32_t[1];
    start_node.current_path[0] = start_vertex;
    start_node.cost = 0;

    // Calculate the initial lower bound using the improved function
    start_node.lower_bound = start_node.cost + calculateLowerBound(graph, start_node, start_vertex);

    bfs_queue.push(start_node);

    int32_t upper_bound = std::numeric_limits<int32_t>::max();
    std::vector<int32_t> optimal_path;

    while (!bfs_queue.empty()) {
        Node current_node = bfs_queue.front();
        bfs_queue.pop();

        // Prune nodes whose lower bound exceeds the current best cost
        if (current_node.lower_bound >= upper_bound)
            continue;

        // If it's a leaf node
        if (current_node.current_path_length == n) {
            int32_t return_edge_cost = graph[current_node.current_vertex][start_vertex];
            if (return_edge_cost == -1)
                continue;

            int32_t total_cost = current_node.cost + return_edge_cost;

            if (total_cost < upper_bound) {
                upper_bound = total_cost;
                optimal_path.assign(current_node.current_path, current_node.current_path + current_node.current_path_length);
                // optimal_path.push_back(start_vertex); // Return to start
            }
            continue;
        }

        // Generate child nodes for all unvisited vertices
        for (int i = 0; i < n; i++) {
            uint32_t bit_mask = 1 << i;
            if (!(current_node.visited_bits & bit_mask)) {
                int32_t edge_cost = graph[current_node.current_vertex][i];
                if (edge_cost == -1)
                    continue;

                Node child(n);
                child.current_vertex = i;
                child.visited_bits = current_node.visited_bits | bit_mask;
                child.current_path_length = current_node.current_path_length + 1;

                // Allocate and copy current_path
                child.current_path = new int32_t[child.current_path_length];
                std::copy(current_node.current_path, current_node.current_path + current_node.current_path_length, child.current_path);
                child.current_path[child.current_path_length - 1] = i;

                // Update cost and calculate lower_bound using the improved function
                child.cost = current_node.cost + edge_cost;
                child.lower_bound = child.cost + calculateLowerBound(graph, child, start_vertex);

                // Prune nodes whose lower bound exceeds the current best cost
                if (child.lower_bound >= upper_bound)
                    continue;

                bfs_queue.emplace(std::move(child));
            }
        }
    }

    return {upper_bound, optimal_path};
}

std::pair<int32_t, std::vector<int32_t>> algorithmsForTSP::best_first_search(std::vector<std::vector<int32_t>> &graph, bool isDirected, int32_t &start_vertex)
{
    int n = graph.size();
    std::priority_queue<Node, std::deque<Node>> bf_priority_queue;

    Node start_node(n);
    start_node.current_vertex = start_vertex;
    start_node.visited_bits = 1 << start_vertex;
    start_node.current_path_length = 1;
    start_node.current_path = new int32_t[1];
    start_node.current_path[0] = start_vertex;
    start_node.cost = 0;

    // Calculate the initial lower bound using the improved function
    start_node.lower_bound = start_node.cost + calculateLowerBound(graph, start_node, start_vertex);

    bf_priority_queue.push(start_node);

    int32_t upper_bound = std::numeric_limits<int32_t>::max();
    std::vector<int32_t> optimal_path;

    while (!bf_priority_queue.empty()) {
        Node current_node = bf_priority_queue.top();
        bf_priority_queue.pop();

        // Prune nodes whose lower bound exceeds the current best cost
        if (current_node.lower_bound >= upper_bound)
            continue;

        // If it's a leaf node
        if (current_node.current_path_length == n) {
            int32_t return_edge_cost = graph[current_node.current_vertex][start_vertex];
            if (return_edge_cost == -1)
                continue;

            int32_t total_cost = current_node.cost + return_edge_cost;

            if (total_cost < upper_bound) {
                upper_bound = total_cost;
                optimal_path.assign(current_node.current_path, current_node.current_path + current_node.current_path_length);
                // optimal_path.push_back(start_vertex); // Return to start
            }
            continue;
        }

        // Generate child nodes for all unvisited vertices
        for (int i = 0; i < n; i++) {
            uint32_t bit_mask = 1 << i;
            if (!(current_node.visited_bits & bit_mask)) {
                int32_t edge_cost = graph[current_node.current_vertex][i];
                if (edge_cost == -1)
                    continue;

                Node child(n);
                child.current_vertex = i;
                child.visited_bits = current_node.visited_bits | bit_mask;
                child.current_path_length = current_node.current_path_length + 1;

                // Allocate and copy current_path
                child.current_path = new int32_t[child.current_path_length];
                std::copy(current_node.current_path, current_node.current_path + current_node.current_path_length, child.current_path);
                child.current_path[child.current_path_length - 1] = i;

                // Update cost and calculate lower_bound using the improved function
                child.cost = current_node.cost + edge_cost;
                child.lower_bound = child.cost + calculateLowerBound(graph, child, start_vertex);

                // Prune nodes whose lower bound exceeds the current best cost
                if (child.lower_bound >= upper_bound)
                    continue;

                bf_priority_queue.emplace(std::move(child));
            }
        }
    }

    return {upper_bound, optimal_path};
}
