#include "FlowNetwork.h"
#include "FindMaxFlowTest.h"
#include "Image.h"
#include "ImageSegmentationExamples.h"

int main()
{
	//FindMaxFlowTest::test_1();
	//FindMaxFlowTest::test_2();
	//FindMaxFlowTest::test_3();
	//FindMaxFlowTest::test_4();

	//ImageSegmentationExamples::banana();
	//ImageSegmentationExamples::fullmoon();

	Image image("Images/fullmoon-gr.jpg", "BackgroundPixels/fullmoon-gr.txt", "ObjectPixels/fullmoon-gr.txt");

	const int sigma{ 2 };
	const double lambda{ .5 };
	const bool eight_edges{ false };

	image.segment_image(sigma, lambda, eight_edges);
	image.save_image("SegmentedImages/fullmoon-gr.jpg");
}