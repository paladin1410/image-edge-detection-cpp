#include "Image.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <limits>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(std::vector<uint8_t> data, int width, int height, int channels) 
    : data(std::move(data)), width(width), height(height), channels(channels) {
    
    // Basic validation
    if (width <= 0 || height <= 0 || channels <= 0) {
        throw std::invalid_argument("Invalid image dimensions or channel count");
    }
    
    size_t expectedSize = static_cast<size_t>(width) * height * channels;
    if (this->data.size() != expectedSize) {
        throw std::invalid_argument("Data size doesn't match dimensions");
    }
}


Image Image::loadFromFile(const std::string& filepath) {
    // Validate file path
    if (filepath.empty()) {
        throw std::invalid_argument("File path cannot be empty");
    }

    // Check file existence and permissions
    std::error_code ec;
    if (!std::filesystem::exists(filepath, ec)) {
        throw std::runtime_error("File does not exist: " + filepath);
    }
    
    if (ec) {
        throw std::runtime_error("Error accessing file: " + filepath + " (" + ec.message() + ")");
    }

    // Image property variables
    int width, height, channels;
    
    // Load image using STB with error checking
    unsigned char* raw_data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!raw_data) {
        std::string stb_error = stbi_failure_reason() ? stbi_failure_reason() : "Unknown STB error";
        throw std::runtime_error("Failed to load image '" + filepath + "': " + stb_error);
    }

    // Validate image dimensions
    if (width <= 0 || height <= 0) {
        stbi_image_free(raw_data);
        throw std::runtime_error("Invalid image dimensions: " + std::to_string(width) + "x" + std::to_string(height));
    }

    // Check for edge detection minimum requirements
    if (width < 3 || height < 3) {
        stbi_image_free(raw_data);
        throw std::runtime_error("Image too small for edge detection (minimum 3x3): " + 
                                std::to_string(width) + "x" + std::to_string(height));
    }

    // Validate channel count
    if (channels < 1 || channels > 4) {
        stbi_image_free(raw_data);
        throw std::runtime_error("Unsupported channel count: " + std::to_string(channels) + 
                                " (supported: 1-4 channels)");
    }

    // Check memory requirements
    size_t dataSize = static_cast<size_t>(width) * height * channels;
    constexpr size_t MAX_IMAGE_SIZE = 100 * 1024 * 1024; // 100MB limit
    if (dataSize > MAX_IMAGE_SIZE) {
        stbi_image_free(raw_data);
        throw std::runtime_error("Image too large: " + std::to_string(dataSize) + 
                                " bytes (limit: " + std::to_string(MAX_IMAGE_SIZE) + ")");
    }

    // Copy raw data into a std::vector
    std::vector<uint8_t> pixel_data(raw_data, raw_data + dataSize);

    // Free the original STB data 
    stbi_image_free(raw_data);
              
    // Return the Image object
    return Image(pixel_data, width, height, channels);
}

void Image::saveToFile(const std::string& filepath) const {
    // Validate parameters
    if (filepath.empty()) {
        throw std::invalid_argument("File path cannot be empty");
    }

    if (data.empty() || width <= 0 || height <= 0) {
        throw std::runtime_error("Cannot save invalid image data");
    }

    // Check directory exists and is writable
    std::filesystem::path path(filepath);
    std::error_code ec;
    if (path.has_parent_path()) {
        auto parent = path.parent_path();
        if (!std::filesystem::exists(parent, ec)) {
            throw std::runtime_error("Directory does not exist: " + parent.string());
        }
        if (ec) {
            throw std::runtime_error("Error accessing directory: " + ec.message());
        }
    }

    // Validate data consistency
    size_t expectedSize = static_cast<size_t>(width) * height * channels;
    if (data.size() != expectedSize) {
        throw std::runtime_error("Image data size mismatch. Expected: " + 
                                std::to_string(expectedSize) + ", Actual: " + 
                                std::to_string(data.size()));
    }

    // Save as PNG (for both grayscale and color)
    int result = stbi_write_png(filepath.c_str(), width, height, channels, 
                               data.data(), width * channels);
    if (!result) {
        throw std::runtime_error("Failed to save image: " + filepath + 
                                " (possible: disk full, permission denied, or invalid path)");
    }

}

Image Image::toGrayscale() const {
    // Validate input
    if (data.empty() || width <= 0 || height <= 0) {
        throw std::runtime_error("Cannot convert invalid image to grayscale");
    }

    // If the image is already grayscale, return a copy of the current object
    if (channels == 1) {
        return *this;
    }

    // Only support RGB/RGBA conversion
    if (channels != 3 && channels != 4) {
        throw std::runtime_error("Grayscale conversion only supports RGB (3 channels) or RGBA (4 channels). "
                                "Current channels: " + std::to_string(channels));
    }

    // Validate data size consistency
    size_t expectedSize = static_cast<size_t>(width) * height * channels;
    if (data.size() != expectedSize) {
        throw std::runtime_error("Image data corrupted. Expected size: " +
                                std::to_string(expectedSize) + ", Actual: " +
                                std::to_string(data.size()));
    }

    // Create a new vector to hold the grayscale pixel data
    std::vector<uint8_t> gray_data(width * height);

    // Convert RGB to grayscale using the luminosity formula
    for (int i = 0; i < width * height; ++i) {
        uint8_t r = data[static_cast<size_t>(i) * channels + 0];
        uint8_t g = data[static_cast<size_t>(i) * channels + 1];
        uint8_t b = data[static_cast<size_t>(i) * channels + 2];
        
        // Luminosity formula
        gray_data[i] = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
    }

    // Use the new constructor to create and return the grayscale Image object
    return Image(gray_data, width, height, 1);
}