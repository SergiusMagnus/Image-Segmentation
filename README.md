# Image Segmentation
This project implements interactive segmentation of black and white images using graph theory.

## How It Works
The image is presented as a weighted directed graph. Each pixel is treated as the vertices of the graph and is connected to neighboring pixels. Also, a source is added from which edges are drawn to ordinary vertices, and a sink vertex to which edges are drawn from ordinary vertices. The edge weights depend on the color intensity of the incident vertices. In the resulting graph, the minimum cut is determined. Thanks to which each pixel is classified as either a background or an object.

## Dependencies
- OpenCV - 4.5.5