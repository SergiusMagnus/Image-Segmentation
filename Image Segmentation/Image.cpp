#include "Image.h"

bool Image::is_pixel_background(int x, int y)
{
	for (Rectangle& rectangle : background_pixels)
	{
		if ((rectangle.x1 <= x) && (rectangle.y1 <= y) && (rectangle.x2 >= x) && (rectangle.y2 >= y))
		{
			return true;
		}
	}
	return false;
}

bool Image::is_pixel_object(int x, int y)
{
	for (Rectangle& rectangle : object_pixels)
	{
		if ((rectangle.x1 <= x) && (rectangle.y1 <= y) && (rectangle.x2 >= x) && (rectangle.y2 >= y))
		{
			return true;
		}
	}
	return false;
}

Image::Image(std::string image_path, std::string background_file_path, std::string object_file_path)
{
	image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	if (image.empty())
	{
		std::cerr << "Could not read the image: " << image_path << std::endl;
		exit(1);
	}

	width = image.cols;
	height = image.rows;

	set_background_pixels(background_file_path);
	set_object_pixels(object_file_path);
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

	int number_of_background_pixels{ std::accumulate(background_pixels_histogram.cbegin(), background_pixels_histogram.cend(), 0) };

	for (int i{ 0 }; i < background_pixels_histogram.size(); ++i)
	{
		background_pixels_probability[i] = background_pixels_histogram[i] / static_cast<double>(number_of_background_pixels);
		if (background_pixels_probability[i] == 0)
		{
			background_pixels_probability[i] = 1e-6;
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

	int number_of_object_pixels{ std::accumulate(object_pixels_histogram.cbegin(), object_pixels_histogram.cend(), 0) };

	for (int i{ 0 }; i < object_pixels_histogram.size(); ++i)
	{
		object_pixels_probability[i] = object_pixels_histogram[i] / static_cast<double>(number_of_object_pixels);
		if (object_pixels_probability[i] == 0)
		{
			object_pixels_probability[i] = 1e-6;
		}
	}
}

void Image::build_network(int sigma, double lambda, bool eight_edges)
{
	int source{ 0 };
	int sink{ width * height + 1 };

	set_source_sink_points_number(0, width * height + 1, width * height + 2);

	// edge from source
	for (int i{ 0 }; i < height; ++i)
	{
		for (int j{ 0 }; j < width; ++j)
		{
			int current_point{ i * width + j + 1 };

			int edge_capacity{};

			if (is_pixel_background(i, j))
			{
				edge_capacity = 0;
			}
			else if (is_pixel_object(i, j))
			{
				edge_capacity = INT_MAX;
			}
			else
			{
				edge_capacity = static_cast<int>(-lambda * log(background_pixels_probability[image.at<uchar>(i, j)]));
			}

			add_edge(source, current_point, edge_capacity);
		}
	}

	// usual edge
	for (int i{ 0 }; i < height; ++i)
	{
		for (int j{ 0 }; j < width; ++j)
		{
			int current_point{ i * width + j + 1 };
			int current_point_intensity{ image.at<uchar>(i, j) };

			// edge to point on right
			if (j != width - 1)
			{
				int point_on_right{ i * width + j + 1 + 1 };
				int point_on_right_intensity{ image.at<uchar>(i, j + 1) };
				double distance{ 1. };

				int edge_capacity{ static_cast<int>(exp(-pow(current_point_intensity - point_on_right_intensity, 2) / (2 * pow(sigma, 2))) * 1 / distance)};

				add_edge(current_point, point_on_right, edge_capacity);
			}

			// edge to point below
			if (i != height - 1)
			{
				int point_below{ (i + 1) * width + j + 1 };
				int point_on_right_intensity{ image.at<uchar>(i + 1, j) };
				double distance{ 1. };

				int edge_capacity{ static_cast<int>(exp(-pow(current_point_intensity - point_on_right_intensity, 2) / (2 * pow(sigma, 2))) * 1 / distance) };

				add_edge(current_point, point_below, edge_capacity);
			}

			// edge to point on left
			if (j != 0)
			{
				int point_on_left{ i * width + j };
				int point_on_right_intensity{ image.at<uchar>(i, j - 1) };
				double distance{ 1. };

				int edge_capacity{ static_cast<int>(exp(-pow(current_point_intensity - point_on_right_intensity, 2) / (2 * pow(sigma, 2))) * 1 / distance) };

				add_edge(current_point, point_on_left, edge_capacity);
			}

			// edge to point on top
			if (i != 0)
			{
				int point_on_top{ (i - 1) * width + j + 1 };
				int point_on_right_intensity{ image.at<uchar>(i - 1, j) };
				double distance{ 1. };

				int edge_capacity{ static_cast<int>(exp(-pow(current_point_intensity - point_on_right_intensity, 2) / (2 * pow(sigma, 2))) * 1 / distance) };

				add_edge(current_point, point_on_top, edge_capacity);
			}
		}
	}

	// edge to sink
	for (int i{ 0 }; i < height; ++i)
	{
		for (int j{ 0 }; j < width; ++j)
		{
			int current_point{ i * width + j + 1 };

			int edge_capacity{};

			if (is_pixel_background(i, j))
			{
				edge_capacity = INT_MAX;
			}
			else if (is_pixel_object(i, j))
			{
				edge_capacity = 0;
			}
			else
			{
				edge_capacity = static_cast<int>(-lambda * log(object_pixels_probability[image.at<uchar>(i, j)]));
			}

			add_edge(current_point, sink, edge_capacity);
		}
	}

	set_edge_incident_to_point();
}

void Image::segment_image()
{
	for (int i{ 0 }; i < is_point_reachable.size(); ++i)
	{
		int x{ 0 };
		int y{ 0 };
		int point_number{ i + 1 };

		while (point_number > width)
		{
			point_number -= width;
			++x;
		}
		y = point_number - 1;

		if (is_point_reachable[i])
		{
			image.at<uchar>(x, y) = 255;
		}
		else
		{
			image.at<uchar>(x, y) = 0;
		}
	}

	cv::imshow("Window", image);
	cv::waitKey();
}