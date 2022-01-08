#include "Image.h"

Image::Image(std::string image_path)
{
	image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	if (image.empty())
	{
		std::cerr << "Could not read the image: " << image_path << std::endl;
		exit(1);
	}

	width = image.cols;
	height = image.rows;
}

void Image::set_background_pixels(std::string file_path)
{
	std::ifstream inf{ file_path };

	if (!inf)
	{
		std::cerr << file_path << " not found." << std::endl;
		exit(1);
	}

	int rectangle_number{};
	inf >> rectangle_number;

	Rectangle current_rectangle{ 0, 0, 0, 0 };

	for (int i{ 0 }; i < rectangle_number; ++i)
	{
		inf >> current_rectangle.x1 >> current_rectangle.y1 >> current_rectangle.x2 >> current_rectangle.y2;
		background_pixels.emplace_back(current_rectangle);
	}

	inf.close();

	background_pixels_histogram.fill(0);

	for (const Rectangle& rectangle : background_pixels)
	{
		for (int i{ rectangle.x1 }; i < rectangle.x2; ++i)
		{
			for (int j{ rectangle.y1 }; j < rectangle.y2; ++j)
			{
				++background_pixels_histogram[image.at<uchar>(i, j)];
			}
		}
	}
}

void Image::set_object_pixels(std::string file_path)
{
	std::ifstream inf{ file_path };

	if (!inf)
	{
		std::cerr << file_path << " not found." << std::endl;
		exit(1);
	}

	int rectangle_number{};
	inf >> rectangle_number;

	Rectangle current_rectangle{ 0, 0, 0, 0 };

	for (int i{ 0 }; i < rectangle_number; ++i)
	{
		inf >> current_rectangle.x1 >> current_rectangle.y1 >> current_rectangle.x2 >> current_rectangle.y2;
		object_pixels.emplace_back(current_rectangle);
	}

	inf.close();

	object_pixels_histogram.fill(0);

	for (const Rectangle& rectangle : object_pixels)
	{
		for (int i{ rectangle.x1 }; i < rectangle.x2; ++i)
		{
			for (int j{ rectangle.y1 }; j < rectangle.y2; ++j)
			{
				++object_pixels_histogram[image.at<uchar>(i, j)];
			}
		}
	}
}