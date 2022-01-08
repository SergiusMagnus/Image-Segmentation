#include "FlowNetwork.h"

bool FlowNetwork::BFS()
{
	shortest_path_to_point.clear();
	shortest_path_to_point.resize(number_of_points, -1);
	shortest_path_to_point[source] = 0;

	std::queue<int> Q;
	Q.emplace(source);

	while (!Q.empty())
	{
		int current_point{ Q.front() };
		Q.pop();

		for (const Edge* edge : edges_incident_to_point[current_point])
		{
			if ((edge->point_a == current_point) && (edge->capacity > edge->flow) && (shortest_path_to_point[edge->point_b] == -1))
			{
				shortest_path_to_point[edge->point_b] = shortest_path_to_point[edge->point_a] + 1;
				Q.emplace(edge->point_b);
			}
			else if ((edge->point_b == current_point) && (edge->flow > 0) && (shortest_path_to_point[edge->point_a] == -1))
			{
				shortest_path_to_point[edge->point_a] = shortest_path_to_point[edge->point_b] + 1;
				Q.emplace(edge->point_a);
			}
		}
	}
	return (shortest_path_to_point[sink] != -1) ? true : false;
}

int FlowNetwork::DFS(int current_point, int min_capacity)
{
	if ((current_point == sink) || (min_capacity == 0))
	{
		return min_capacity;
	}

	for (Edge* edge : edges_incident_to_point[current_point])
	{
		if ((edge->point_a == current_point) && (edge->capacity > edge->flow) && (shortest_path_to_point[edge->point_a] + 1 == shortest_path_to_point[edge->point_b]))
		{
			int delta{ DFS(edge->point_b, std::min(min_capacity, edge->capacity - edge->flow)) };

			if (delta != 0)
			{
				edge->flow += delta;
				return delta;
			}
		}
		else if ((edge->point_b == current_point) && (edge->flow > 0) && (shortest_path_to_point[edge->point_a] == shortest_path_to_point[edge->point_b] + 1))
		{
			int delta{ DFS(edge->point_a, std::min(min_capacity, edge->flow)) };
			
			if (delta != 0)
			{
				edge->flow -= delta;
				return delta;
			}
		}
	}
	return 0;
}

FlowNetwork::FlowNetwork(std::string file_path)
{
	std::ifstream inf{ file_path };

	if (!inf)
	{
		std::cerr << file_path << " not found." << std::endl;
		exit(1);
	}

	inf >> number_of_points >> number_of_edges;

	list_of_edges.reserve(number_of_edges);

	Edge current_edge{ 0, 0, 0, 0 };

	for (int i{ 0 }; i < number_of_edges; ++i)
	{
		inf >> current_edge.point_a >> current_edge.point_b >> current_edge.capacity;

		list_of_edges.emplace_back(current_edge);
	}

	inf.close();

	source = list_of_edges.front().point_a;
	sink = number_of_points - 1;

	shortest_path_to_point.reserve(number_of_points);
	first_edge_number_from_point.reserve(number_of_points);
	edges_incident_to_point.resize(number_of_points);

	// if first point equals 1 (must equal 0)
	if (source == 1)
	{
		source = 0;

		for (Edge& edge : list_of_edges)
		{
			--edge.point_a;
			--edge.point_b;
		}
	}

	for (Edge& edge : list_of_edges)
	{
		edges_incident_to_point[edge.point_a].emplace_back(&edge);
		edges_incident_to_point[edge.point_b].emplace_back(&edge);
	}
}

int FlowNetwork::find_max_flow()
{
	int max_flow{ 0 };
	int flow{};

	while (BFS())
	{
		while (flow = DFS(source, INT_MAX))
		{
			max_flow += flow;
		}
	}
	return max_flow;
}

void FlowNetwork::print_list_of_edges()
{
	std::cout << "Network" << std::endl;
	for (const Edge& edge : list_of_edges)
	{
		std::cout << edge.point_a << ' ' << edge.point_b << ' '
			<< edge.capacity << ' ' << edge.flow << std::endl;
	}
	std::cout << std::endl;
}
