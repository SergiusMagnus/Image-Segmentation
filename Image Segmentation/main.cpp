#include <iostream>
#include <string>

#include "FlowNetwork.h"

int main()
{
	std::cout << "start" << std::endl;

	FlowNetwork test_graph{ std::string{ "test_graph.txt" } };
	std::cout << test_graph.find_max_flow() << std::endl;

	std::cout << "Function main() has completed.";
}