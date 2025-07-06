#pragma once
#include "Image.h"
#include <string>

/**
 * EdgeDetector implements Sobel and Prewitt edge detection algorithms.
 * Uses 3x3 convolution kernels to detect image gradients and calculate edge magnitude.
 * Automatically handles grayscale conversion and boundary padding.
 */
class EdgeDetector {
public:
    /**
     * Detects edges in an image using specified operator
     * @param image Input image (any format - automatically converted to grayscale)
     * @param operatorName "Sobel" or "Prewitt" (case-insensitive)
     * @return New grayscale Image with detected edges (white=edges, black=no edges)
     * @throws invalid_argument for unknown operators
     * @throws runtime_error for images < 3x3 pixels
     */
    static Image detectEdges(const Image& image, const std::string& operatorName);

private:
    // Sobel operator kernels for gradient calculation
    static const int SOBEL_X[3][3];   // Horizontal edge detection
    static const int SOBEL_Y[3][3];   // Vertical edge detection
    
    // Prewitt operator kernels for gradient calculation  
    static const int PREWITT_X[3][3]; // Horizontal edge detection
    static const int PREWITT_Y[3][3]; // Vertical edge detection
    
    /**
     * Creates padded image to handle boundary conditions during convolution
     * Uses border replication to extend image edges
     */
    static std::vector<uint8_t> createPaddedImage(const std::vector<uint8_t>& originalData,
                                                  int width, int height, int padSize = 1);
    
    /**
     * Applies 3x3 convolution kernel at specified position
     * @return Convolution result (gradient component)
     */
    static int applyKernel(const std::vector<uint8_t>& imageData, int width, int x, int y, 
                          const int kernel[3][3]);
    
    /**
     * Calculates edge magnitude from gradient components using Euclidean norm
     * @param gx Horizontal gradient component
     * @param gy Vertical gradient component  
     * @return Edge magnitude clamped to [0, 255]
     */
    static uint8_t calculateMagnitude(int gx, int gy);
};