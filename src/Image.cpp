#include "Image.h"        

#include <iostream>       
#include <stdexcept>      

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION  
#include "stb_image_write.h"

Image Image::loadFromFile(const std::string& filepath) {
    Image img;
    
    // Load image using STB
    unsigned char* data = stbi_load(filepath.c_str(), &img.width, &img.height, &img.channels, 0);
    
    if (!data) {
        throw std::runtime_error("Failed to load image: " + filepath);
    }
    
    // Copy data to vector
    size_t dataSize = img.width * img.height * img.channels;
    img.data.assign(data, data + dataSize);
    
    // Free STB memory
    stbi_image_free(data);
    
    std::cout << "Loaded image: " << img.width << "x" << img.height << " (" << img.channels << " channels)" << std::endl;
    
    return img;
}

void Image::saveToFile(const std::string& filepath) const {
    // Save as grayscale PNG 
    int result = stbi_write_png(filepath.c_str(), width, height, channels, data.data(), width * channels);
    
    if (!result) {
        throw std::runtime_error("Failed to save image: " + filepath);
    }
    
    std::cout << "Saved grayscale image: " << filepath << std::endl;
}

Image Image::toGrayscale() const {
    // Already grayscale, no conversion is needed
    if (channels == 1) {
        return *this; 
    }
    
    Image grayImg;
    grayImg.width = width;
    grayImg.height = height;
    grayImg.channels = 1;
    grayImg.data.resize(width * height);
    
    // Convert RGB to grayscale using luminosity formula
    for (int i = 0; i < width * height; ++i) {
        uint8_t r = data[i * channels + 0];
        uint8_t g = data[i * channels + 1];
        uint8_t b = data[i * channels + 2];
        
        // Standard luminosity formula
        grayImg.data[i] = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
    }
    
    std::cout << "Converted to grayscale" << std::endl;
    
    return grayImg;
}
