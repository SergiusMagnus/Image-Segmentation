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
	std::vector<int> accessible_edge_from_point;

	bool BFS();
	int DFS(int current_point, int min_capacity);

protected:
	std::vector<bool> is_point_reachable;

	FlowNetwork();
	FlowNetwork(std::string file_path);

	void add_edge(int point_a, int point_b, int capacity);
	void set_edge_incident_to_point();
	void set_source_sink_points_number(int source, int sink, int number_of_points);

public:
	int find_max_flow();

	friend class FindMaxFlowTest;
};

#endif