#include "ImageSegmentationExamples.h"

void ImageSegmentationExamples::banana()
{
	Image image("Images/banana1-gr.jpg", "BackgroundPixels/banana1-gr.txt", "ObjectPixels/banana1-gr.txt");

	const int sigma{ 2 };
	const double lambda{ .0725 };
	const bool eight_edges{ false };

	image.segment_image(sigma, lambda, eight_edges);
	image.save_image("SegmentedImages/banana1-gr.jpg");
}

void ImageSegmentationExamples::fullmoon()
{
	Image image("Images/fullmoon-gr.jpg", "BackgroundPixels/fullmoon-gr.txt", "ObjectPixels/fullmoon-gr.txt");

	const int sigma{ 2 };
	const double lambda{ .5 };
	const bool eight_edges{ false };

	image.segment_image(sigma, lambda, eight_edges);
	image.save_image("SegmentedImages/fullmoon-gr.jpg");
}