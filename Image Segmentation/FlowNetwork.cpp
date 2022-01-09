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

	for (int i{ accessible_edge_from_point[current_point] }; i < edges_incident_to_point[current_point].size(); ++i)
	{
		Edge* edge{ edges_incident_to_point[current_point][i] };

		if ((edge->point_a == current_point) && (edge->capacity > edge->flow) && (shortest_path_to_point[edge->point_a] + 1 == shortest_path_to_point[edge->point_b]))
		{
			int delta{ DFS(edge->point_b, std::min(min_capacity, edge->capacity - edge->flow)) };

			if (delta != 0)
			{
				edge->flow += delta;
				if (edge->point_b != sink)
				{
					is_point_reachable[edge->point_b - 1] = true;
				}
				return delta;
			}
		}
		else if ((edge->point_b == current_point) && (edge->flow > 0) && (shortest_path_to_point[edge->point_a] == shortest_path_to_point[edge->point_b] + 1))
		{
			int delta{ DFS(edge->point_a, std::min(min_capacity, edge->flow)) };
			
			if (delta != 0)
			{
				edge->flow -= delta;
				if (edge->point_a != sink)
				{
					is_point_reachable[edge->point_a - 1] = true;
				}
				return delta;
			}
		}
		++accessible_edge_from_point[current_point];
	}
	return 0;
}

FlowNetwork::FlowNetwork()
{
	number_of_points = 0;
	number_of_edges = 0;
	source = 0;
	sink = 0;
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

	Edge current_edge{ 0, 0, 0., 0. };

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
	accessible_edge_from_point.reserve(number_of_points);

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

void FlowNetwork::set_source_sink_points_number(int source, int sink, int number_of_points)
{
	this->source = source;
	this->sink = sink;
	this->number_of_points = number_of_points;

	shortest_path_to_point.reserve(number_of_points);
	first_edge_number_from_point.reserve(number_of_points);
	edges_incident_to_point.resize(number_of_points);
	accessible_edge_from_point.reserve(number_of_points);
}

void FlowNetwork::add_edge(int point_a, int point_b, int capacity)
{
	list_of_edges.emplace_back(Edge{ point_a, point_b, capacity, 0 });

	++number_of_edges;
}

void FlowNetwork::set_edge_incident_to_point()
{
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
		accessible_edge_from_point.clear();
		accessible_edge_from_point.resize(number_of_points, 0);
		is_point_reachable.clear();
		is_point_reachable.resize(number_of_points - 2, false);

		while (flow = DFS(source, INT_MAX))
		{
			max_flow += flow;
		}
	}
	return max_flow;
}
