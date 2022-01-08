#include "Image.h"

Image::Image(std::string image_path)
{
	image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	if (image.empty())
	{
		std::cerr << "Could not read the image: " << image_path << std::endl;
		exit(1);
	}

	// cv::imshow("Display window", image);
	// cv::waitKey(0);
}