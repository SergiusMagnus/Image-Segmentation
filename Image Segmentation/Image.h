#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <fstream>
#include <iostream>
#include <array>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

class Image
{
private:
	cv::Mat image;

	std::array<int, 255> histogram;


public:
	Image(std::string image_path);

};

#endif