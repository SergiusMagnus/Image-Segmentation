# Image Segmentation
This project implements interactive segmentation of black and white images using graph theory.

## Description of the Logic of the Program
The image is presented as a weighted directed graph. Each pixel is treated as the vertices of the graph and is connected to neighboring pixels. Also, a source is added from which edges are drawn to ordinary vertices, and a sink vertex to which edges are drawn from ordinary vertices. The edge weights depend on the color intensity of the incident vertices. In the resulting graph, the minimum cut is determined. Thanks to which each pixel is classified as either a background or an object.

## Dependencies
- OpenCV - 4.5.5

## How to Use
The user needs to indicate in the corresponding txt files the pixels that exactly belong to the object and the background, respectively. Pixels are defined by rectangles. File record form: in the first line indicates the number of rectangles; then in each line the coordinates of one rectangle are indicated separated by a space (coordinates of the upper left pixel, coordinates of the lower right pixel).
#### Example
    2
    305 170 330 320
    315 395 360 415
To segment an image, you need to create an object of the Image class. As parameters, you must specify: the address of the image file, the address of the file of pixels related to the background, the address of the file of pixels related to objects. <br/> Next, you need to call a segment_image method of an object of the Image class with three parameters. The first two are the coefficients used when calculating the weights of the edges, the third is a boolean variable, with the False value, the edges are built between the pixels only horizontally and vertically, with True, the edges are also built between the pixels along the diagonal. <br/> The save_image method with the file address saves the image.
#### Example
```c++
Image image("Images/fullmoon-gr.jpg", "BackgroundPixels/fullmoon-gr.txt", "ObjectPixels/fullmoon-gr.txt");

const int sigma{ 2 };
const double lambda{ .5 };
const bool eight_edges{ false };

image.segment_image(sigma, lambda, eight_edges);
image.save_image("SegmentedImages/fullmoon-gr.jpg");
```
## An Example of Image Segmentation
[Original image](Images/fullmoon-gr.jpg)
