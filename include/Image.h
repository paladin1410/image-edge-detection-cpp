#pragma once  

#include <string>    
#include <vector>    
#include <cstdint>   

class Image {
    public:
        Image(std::vector<uint8_t> data, int width, int height, int channels);
        static Image loadFromFile(const std::string& filepath);
        void saveToFile(const std::string& filepath) const;
        Image toGrayscale() const;

        // Helper functions needed for edge detection
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getChannels() const { return channels; }
        const std::vector<uint8_t>& getData() const { return data; }

    private:
        std::vector<uint8_t> data;
        int width, height, channels;
};
