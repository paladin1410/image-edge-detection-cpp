#include "EdgeDetector.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <cctype>     
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

    // Validate operator name first
    std::string lowerOp = operatorName;
    std::transform(lowerOp.begin(), lowerOp.end(), lowerOp.begin(), ::tolower);
    if (lowerOp != "sobel" && lowerOp != "prewitt") {
        throw std::invalid_argument("Unknown edge detection operator: " + operatorName + 
                                  ". Supported operators: 'Sobel', 'Prewitt' (case-insensitive)");
    }
    
    // Validate image dimensions before processing
    int width = image.getWidth();
    int height = image.getHeight();
    
    if (width < 3 || height < 3) {
        throw std::runtime_error("Image too small for edge detection. Minimum size: 3x3, "
                                "Actual size: " + std::to_string(width) + "x" + std::to_string(height));
    }
    
    // Validate image data integrity
    const std::vector<uint8_t>& originalData = image.getData();
    size_t expectedSize = static_cast<size_t>(width) * height * image.getChannels();
    if (originalData.empty() || originalData.size() != expectedSize) {
        throw std::runtime_error("Invalid image data. Expected size: " + std::to_string(expectedSize) + 
                                ", Actual size: " + std::to_string(originalData.size()));
    }

    // Edge detection works on grayscale images.
    Image grayImage = image.toGrayscale();

    const std::vector<uint8_t>& imageData = grayImage.getData();

    // Select the appropriate kernels based on the operator name.
    const int (*kernelX)[3];
    const int (*kernelY)[3];

    if (lowerOp == "sobel") {
        kernelX = SOBEL_X;
        kernelY = SOBEL_Y;
    } else {
        kernelX = PREWITT_X;
        kernelY = PREWITT_Y;
    } 

    // Create padded image
    std::vector<uint8_t> paddedData = createPaddedImage(imageData, width, height, 1);
    int paddedWidth = width + 2;
    
    // Apply edge detection on padded image
    std::vector<uint8_t> resultData(width * height);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Coordinates in padded image (offset by 1)
            int paddedX = x + 1;
            int paddedY = y + 1;
            
            // Apply kernels
            int gx = applyKernel(paddedData, paddedWidth, paddedX, paddedY, kernelX);
            int gy = applyKernel(paddedData, paddedWidth, paddedX, paddedY, kernelY);
            
            // Calculate magnitude and store in original coordinates
            uint8_t magnitude = calculateMagnitude(gx, gy);
            resultData[y * width + x] = magnitude;
        }
    }

    // Return a new Image object with the edge data.
    return Image(resultData, width, height, 1);
}

std::vector<uint8_t> EdgeDetector::createPaddedImage(const std::vector<uint8_t>& originalData,
                                                     int width, int height, int padSize) {
    int paddedWidth = width + 2 * padSize;
    int paddedHeight = height + 2 * padSize;
    std::vector<uint8_t> paddedData(paddedWidth * paddedHeight);

    // Copy original image to center of padded image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int originalIndex = y * width + x;
            int paddedIndex = (y + padSize) * paddedWidth + (x + padSize);
            paddedData[paddedIndex] = originalData[originalIndex];
        }
    }

    // Replicate top and bottom borders
    for (int y = 0; y < padSize; ++y) {
        for (int x = padSize; x < paddedWidth - padSize; ++x) {
            // Top border: replicate first row
            paddedData[y * paddedWidth + x] = 
                paddedData[padSize * paddedWidth + x];
            
            // Bottom border: replicate last row
            paddedData[(paddedHeight - 1 - y) * paddedWidth + x] = 
                paddedData[(paddedHeight - 1 - padSize) * paddedWidth + x];
        }
    }
    
    // Replicate left and right borders
    for (int y = 0; y < paddedHeight; ++y) {
        for (int x = 0; x < padSize; ++x) {
            // Left border: replicate first column
            paddedData[y * paddedWidth + x] = 
                paddedData[y * paddedWidth + padSize];
            
            // Right border: replicate last column
            paddedData[y * paddedWidth + (paddedWidth - 1 - x)] = 
                paddedData[y * paddedWidth + (paddedWidth - 1 - padSize)];
        }
    }

    return paddedData;
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