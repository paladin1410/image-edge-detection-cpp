#include "EdgeDetector.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

// Sobel kernels
const int EdgeDetector::SOBEL_X[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};
const int EdgeDetector::SOBEL_Y[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    { 1,  2,  1}
};

// Prewitt kernels
const int EdgeDetector::PREWITT_X[3][3] = {
    {-1, 0, 1},
    {-1, 0, 1},
    {-1, 0, 1}
};
const int EdgeDetector::PREWITT_Y[3][3] = {
    {-1, -1, -1},
    { 0,  0,  0},
    { 1,  1,  1}
};

Image EdgeDetector::detectEdges(const Image& image, const std::string& operatorName) {
    // Edge detection works on grayscale images.
    Image grayImage = image.toGrayscale();

    int width = grayImage.getWidth();
    int height = grayImage.getHeight();
    const std::vector<uint8_t>& imageData = grayImage.getData();

    // Select the appropriate kernels based on the operator name.
    const int (*kernelX)[3];
    const int (*kernelY)[3];

    if (operatorName == "Sobel") {
        kernelX = SOBEL_X;
        kernelY = SOBEL_Y;
    } else if (operatorName == "Prewitt") {
        kernelX = PREWITT_X;
        kernelY = PREWITT_Y;
    } else {
        throw std::invalid_argument("Unknown edge detection operator: " + operatorName);
    }

    std::vector<uint8_t> resultData(width * height, 0);

    // 3. Apply edge detection (skip border pixels)
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int gx = applyKernel(imageData, width, x, y, kernelX);
            int gy = applyKernel(imageData, width, x, y, kernelY);
            uint8_t magnitude = calculateMagnitude(gx, gy);
            resultData[y * width + x] = magnitude;
        }
    }

    // 4. Return a new Image object with the edge data.
    return Image(resultData, width, height, 1);
}

int EdgeDetector::applyKernel(
    const std::vector<uint8_t>& imageData,
    int width,
    int x,
    int y,
    const int kernel[3][3]
) {
    int sum = 0;
    for (int ky = -1; ky <= 1; ++ky) {
        for (int kx = -1; kx <= 1; ++kx) {
            uint8_t pixelValue = imageData[(y + ky) * width + (x + kx)];
            sum += pixelValue * kernel[ky + 1][kx + 1];
        }
    }
    return sum;
}

uint8_t EdgeDetector::calculateMagnitude(int gx, int gy) {
    double magnitude = std::sqrt(static_cast<double>(gx) * gx + static_cast<double>(gy) * gy);
    return static_cast<uint8_t>(std::min(255.0, magnitude));
}