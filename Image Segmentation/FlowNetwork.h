#ifndef FLOWNETWORK_H
#define FLOWNETWORK_H

#include <vector>
#include <queue>
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
	int source;
	int sink;

	std::vector<int> shortest_path_to_point;
	std::vector<int> first_edge_number_from_point;

	std::vector<Edge> list_of_edges;
	std::vector<std::vector<Edge*>> edges_incident_to_point;

	bool BFS();
	int DFS(int current_point, int min_capacity);

public:
	FlowNetwork(std::string file_path);

	int find_max_flow();

	void print_list_of_edges();
};

#endif