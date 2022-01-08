#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <fstream>
#include <iostream>
#include <array>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "Rectangle.h"

class Image
{
private:
	cv::Mat image;

	int width;
	int height;

	std::vector<Rectangle> background_pixels;
	std::vector<Rectangle> object_pixels;

	std::array<int, 256> background_pixels_histogram;
	std::array<int, 256> object_pixels_histogram;


public:
	Image(std::string image_path);

	void set_background_pixels(std::string file_path);
	void set_object_pixels(std::string file_path);
};

#endif