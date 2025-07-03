#include <iostream>       
#include <exception>      
#include "Image.h"        

int main() {
    std::cout << "Edge Detection Program" << std::endl;
    std::cout << "Testing Image I/O functionality..." << std::endl;
    
    try {
        // Test image loading
        std::cout << "\nTesting image loading..." << std::endl;
        Image img = Image::loadFromFile("sample_images/Lenna.png");
        
        // Test grayscale conversion
        std::cout << "\nTesting grayscale conversion..." << std::endl;
        Image grayImg = img.toGrayscale();
        
        // Test image saving
        std::cout << "\nTesting image saving..." << std::endl;
        grayImg.saveToFile("sample_images/Lenna_gray.png");
        
        std::cout << "\n✅ All image operations successful!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ Error: " << e.what() << std::endl;
        std::cout << "Make sure you have Lenna test image at sample_images/Lenna.png" << std::endl;
        return 1;
    }
    
    return 0;
}