#include <iostream>
#include <string>

#include "FlowNetwork.h"
#include "FindMaxFlowTest.h"
#include "Image.h"

int main()
{
	/*FindMaxFlowTest::test_1();
	FindMaxFlowTest::test_2();
	FindMaxFlowTest::test_3();
	FindMaxFlowTest::test_4();*/

	Image image("Images/banana1-gr.jpg", "BackgroundPixels/banana1-gr.txt", "ObjectPixels/banana1-gr.txt");

	const int sigma{ 2 };
	const double lambda{ 0.1 };
	const bool eight_edges{ false };

	image.build_network(sigma, lambda, eight_edges);
	std::cout << image.find_max_flow() << std::endl;
	image.segment_image();
}