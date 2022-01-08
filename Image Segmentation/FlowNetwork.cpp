#include "FlowNetwork.h"

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

		list_of_edges.push_back(current_edge);
	}

	inf.close();

	source = list_of_edges.front().point_a;

	if (source == 0)
	{
		sink = number_of_points - 1;
	}
	else
	{
		sink = number_of_points;
	}
}

int FlowNetwork::find_maximum_flow()
{
	int flow_length{ 0 };
	int maximum_flow{ 0 };

	while (flow_length = BFS_layers_from_source_to_sink())
	{
		int flow{ DFS_flow_from_source_to_sink(flow_length) };

		maximum_flow += flow;

		while (!DFS_flow.empty())
		{
			auto& current_edge{ DFS_flow.top() };

			for (auto& edge : list_of_edges)
			{
				if ((edge.point_a == current_edge.point_a) &&
					(edge.point_b == current_edge.point_b))
				{
					edge.flow += flow;
					break;
				}
				
			}

			DFS_flow.pop();
		}
	}
	return maximum_flow;
}

int FlowNetwork::BFS_layers_from_source_to_sink()
{
	BFS_layers.clear();

	BFS_layers.push_back(std::set<int> { source });

	std::set<int> visited_points{ source };
	std::set<int> next_layer{};

	while (true)
	{
		next_layer.clear();

		for (const auto& edge : list_of_edges)
		{
			if ((edge.capacity - edge.flow != 0) && 
				(BFS_layers.back().find(edge.point_a) != BFS_layers.back().cend()) &&
				(visited_points.find(edge.point_b) == visited_points.cend()))
			{
				next_layer.emplace(edge.point_b);
				visited_points.emplace(edge.point_b);
			}
		}
		if (next_layer.empty())
		{
			break;
		}

		BFS_layers.push_back(next_layer);

		if (next_layer.find(sink) != next_layer.cend())
		{
			BFS_layers.back() = std::set<int>{ sink };
			break;
		}
	}

	if (visited_points.find(sink) == visited_points.cend())
	{
		BFS_layers.clear();
		return BFS_layers.size();
	}
	else
	{
		return BFS_layers.size() - 1;
	}
}

int FlowNetwork::DFS_flow_from_source_to_sink(int flow_length)
{
	int flow{ 0 };

	for (int i{ 0 }; i < flow_length; ++i)
	{
		for (const auto& edge : list_of_edges)
		{
			if (i == 0)
			{
				if ((edge.capacity - edge.flow != 0) &&
					(edge.point_a == source) &&
					(BFS_layers[i + 1].find(edge.point_b) != BFS_layers[i + 1].cend()))
				{
					DFS_flow.emplace(edge);

					flow = edge.capacity - edge.flow;

					break;
				}
			}
			else
			{
				if ((edge.capacity - edge.flow != 0) &&
					(edge.point_a == DFS_flow.top().point_b) &&
					(BFS_layers[i + 1].find(edge.point_b) != BFS_layers[i + 1].cend()))
				{
					DFS_flow.emplace(edge);

					if (flow > edge.capacity - edge.flow)
					{
						flow = edge.capacity - edge.flow;
					}

					break;
				}
			}
		}
	}

	return flow;
}

void FlowNetwork::print_list_of_edges()
{
	for (const auto& edge : list_of_edges)
	{
		std::cout << edge.point_a << ' ' << edge.point_b << ' '
			<< edge.capacity << ' ' << edge.flow << std::endl;
	}
}

void FlowNetwork::print_BFS_layers()
{
	int path_length_from_source_to_sink{ BFS_layers_from_source_to_sink() };

	std::cout << path_length_from_source_to_sink << std::endl;

	for (const auto& layer : BFS_layers)
	{
		for (auto point : layer)
		{
			std::cout << point << ' ';
		}
		std::cout << std::endl;
	}
}

void FlowNetwork::print_DFS_layers()
{
	DFS_flow_from_source_to_sink(BFS_layers_from_source_to_sink());

	std::cout << DFS_flow.size() << std::endl;

	while (!DFS_flow.empty())
	{
		auto edge{ DFS_flow.top() };

		std::cout << edge.point_a << ' ' << edge.point_b << ' '
			<< edge.capacity << ' ' << edge.flow << std::endl;

		DFS_flow.pop();
	}
}