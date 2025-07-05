#pragma once
#include "Image.h"
#include <string>

/**
 * Edge Detector Class with SOBEL and PREWITT Operators
**/

class EdgeDetector {

public:
    static Image detectEdges(const Image& image, const std::string& operatorName);
private:
    // Kernels for different operators
    static const int SOBEL_X[3][3];
    static const int SOBEL_Y[3][3];
    static const int PREWITT_X[3][3];
    static const int PREWITT_Y[3][3];

    // Helper function to apply a 3x3 kernel to a pixel
    static int applyKernel(const std::vector<uint8_t>& imageData, int width, int x, int y, const int kernel[3][3]);

    // Helper function to calculate the gradient magnitude
    static uint8_t calculateMagnitude(int gx, int gy);
};