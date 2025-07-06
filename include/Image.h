#pragma once  
#include <string>    
#include <vector>    
#include <cstdint>   

/**
 * Image class for loading, saving, and processing image data.
 * Supports PNG, JPG formats with RGB/RGBA/Grayscale conversion.
 * Minimum size requirement: 3x3 pixels for edge detection compatibility.
 */
class Image {
public:
    /**
     * Constructor: Creates image from raw pixel data
     * @param data Pixel data as uint8_t vector
     * @param width Image width in pixels
     * @param height Image height in pixels  
     * @param channels Number of color channels (1=grayscale, 3=RGB, 4=RGBA)
     */
    Image(std::vector<uint8_t> data, int width, int height, int channels);
    
    /**
     * Loads image from file using STB library
     * @param filepath Path to image file (PNG, JPG, etc.)
     * @return Image object with loaded data
     * @throws runtime_error if file not found or invalid format
     */
    static Image loadFromFile(const std::string& filepath);
    
    /**
     * Saves image to PNG file
     * @param filepath Output file path
     * @throws runtime_error if save fails
     */
    void saveToFile(const std::string& filepath) const;
    
    /**
     * Converts RGB/RGBA image to grayscale using luminosity formula
     * @return New grayscale Image object 
     * @throws runtime_error if unsupported channel count
     */
    Image toGrayscale() const;
    
    // Accessor methods for image properties
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    const std::vector<uint8_t>& getData() const { return data; }

private:
    std::vector<uint8_t> data;    // Raw pixel data
    int width, height, channels;  // Image dimensions and format
};