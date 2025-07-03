#pragma once  

#include <string>    
#include <vector>    
#include <cstdint>   

class Image {
    public:
        static Image loadFromFile(const std::string& filepath);
        void saveToFile(const std::string& filepath) const;
        Image toGrayscale() const;
    private:
        std::vector<uint8_t> data;
        int width, height, channels;
};
