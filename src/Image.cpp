#include "Image.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <limits>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

    Image img;
    
    // Load image using STB with error checking
    unsigned char* data = stbi_load(filepath.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!data) {
        std::string stb_error = stbi_failure_reason() ? stbi_failure_reason() : "Unknown STB error";
        throw std::runtime_error("Failed to load image '" + filepath + "': " + stb_error);
    }

    // Validate image dimensions
    if (img.width <= 0 || img.height <= 0) {
        stbi_image_free(data);
        throw std::runtime_error("Invalid image dimensions: " + std::to_string(img.width) + "x" + std::to_string(img.height));
    }

    // Check for edge detection minimum requirements
    if (img.width < 3 || img.height < 3) {
        stbi_image_free(data);
        throw std::runtime_error("Image too small for edge detection (minimum 3x3): " + 
                                std::to_string(img.width) + "x" + std::to_string(img.height));
    }

    // Validate channel count
    if (img.channels < 1 || img.channels > 4) {
        stbi_image_free(data);
        throw std::runtime_error("Unsupported channel count: " + std::to_string(img.channels) + 
                                " (supported: 1-4 channels)");
    }

    // Check memory requirements
    size_t dataSize = static_cast<size_t>(img.width) * img.height * img.channels;
    constexpr size_t MAX_IMAGE_SIZE = 100 * 1024 * 1024; // 100MB limit
    if (dataSize > MAX_IMAGE_SIZE) {
        stbi_image_free(data);
        throw std::runtime_error("Image too large: " + std::to_string(dataSize) + 
                                " bytes (limit: " + std::to_string(MAX_IMAGE_SIZE) + ")");
    }

    // Copy data to vector with exception safety
    try {
        img.data.assign(data, data + dataSize);
    } catch (const std::exception& e) {
        stbi_image_free(data);
        throw std::runtime_error("Memory allocation failed for image data: " + std::string(e.what()));
    }

    // Free STB memory
    stbi_image_free(data);

    std::cout << "Loaded image: " << img.width << "x" << img.height 
              << " (" << img.channels << " channels)" << std::endl;
    return img;
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

    std::cout << "Saved image: " << filepath << std::endl;
}

Image Image::toGrayscale() const {
    // Validate input
    if (data.empty() || width <= 0 || height <= 0) {
        throw std::runtime_error("Cannot convert invalid image to grayscale");
    }

    // Check if image is already in grayscale format
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

    Image grayImg;
    grayImg.width = width;
    grayImg.height = height;
    grayImg.channels = 1;

    // Allocate memory with error checking
    try {
        grayImg.data.resize(width * height);
    } catch (const std::exception& e) {
        throw std::runtime_error("Memory allocation failed for grayscale conversion: " + std::string(e.what()));
    }

    // Convert RGB to grayscale using luminosity formula
    for (int i = 0; i < width * height; ++i) {
        uint8_t r = data[i * channels + 0];
        uint8_t g = data[i * channels + 1];
        uint8_t b = data[i * channels + 2];
        
        // Luminosity formula
        grayImg.data[i] = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
    }

    std::cout << "Converted to grayscale" << std::endl;
    return grayImg;
}