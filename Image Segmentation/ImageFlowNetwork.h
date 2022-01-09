#ifndef IMAGEFLOWNETWORK_H
#define IMAGEFLOWNETWORK_H

#include <string>

#include "Image.h"
#include "FlowNetwork.h"

class ImageFlowNetwork : private Image, private FlowNetwork
{
private:

public:
	ImageFlowNetwork(std::string image_path);
};

#endif