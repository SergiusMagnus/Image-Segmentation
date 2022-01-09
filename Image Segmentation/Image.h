#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <numeric>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "Rectangle.h"
#include "FlowNetwork.h"

class Image : public FlowNetwork
{
private:
	cv::Mat image;

	int width;
	int height;

	std::vector<Rectangle> background_pixels;
	std::vector<Rectangle> object_pixels;

	std::array<int, 256> background_pixels_histogram;
	std::array<int, 256> object_pixels_histogram;

	std::array<double, 265> background_pixels_probability;
	std::array<double, 265> object_pixels_probability;

	bool is_pixel_background(int x, int y);
	bool is_pixel_object(int x, int y);

public:
	Image(std::string image_path, std::string background_file_path, std::string object_file_path);

	void set_background_pixels(std::string file_path);
	void set_object_pixels(std::string file_path);

	void build_network(int sigma, double lambda, bool eight_edges);
	void segment_image();
};

#endif