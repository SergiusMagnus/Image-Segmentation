#ifndef FLOWNETWORK_H
#define FLOWNETWORK_H

#include <vector>
#include <set>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "Edge.h"

class FlowNetwork
{
private:
	int number_of_points;
	int number_of_edges;

	std::vector<Edge> list_of_edges;

	int source;
	int sink;


	std::vector<std::set<int>> BFS_layers;

	int BFS_layers_from_source_to_sink();

	std::stack<Edge> DFS_flow;

	int DFS_flow_from_source_to_sink(int flow_length);

public:
	FlowNetwork(std::string file_path);

	int find_maximum_flow();

	void print_list_of_edges();
	void print_BFS_layers();
	void print_DFS_layers();
};

#endif