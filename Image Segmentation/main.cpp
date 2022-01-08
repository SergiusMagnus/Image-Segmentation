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

	Image image("Images/banana1-gr.jpg");
	image.set_background_pixels("BackgroundPixels/banana1-gr.txt");
	image.set_object_pixels("ObjectPixels/banana1-gr.txt");
}