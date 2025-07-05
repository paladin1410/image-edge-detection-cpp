#include <iostream>       
#include <exception>      
#include "Image.h"        
#include "EdgeDetector.h"

int main(int argc, char* argv[]) {
    // Check if image path provided
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <image_path>" << std::endl;
        std::cout << "Example: " << argv[0] << " sample_images/cameraman.jpg" << std::endl;
        return 1;
    }

    std::string imagePath = argv[1];
    
    std::cout << "Testing Image I/O functionality..." << std::endl;
    std::cout << "Input image: " << imagePath << std::endl;
    
    try {
        // Test image loading
        std::cout << "\nTesting image loading..." << std::endl;
        Image img = Image::loadFromFile(imagePath);
        
        // Test grayscale conversion
        std::cout << "\nTesting grayscale conversion..." << std::endl;
        Image grayImg = img.toGrayscale();
        
        // Test image saving  
        std::cout << "\nTesting image saving..." << std::endl;
        grayImg.saveToFile("sample_images/output_gray.png");
        
        // Test Sobel edge detection
        std::cout << "\nTesting Sobel edge detection..." << std::endl;
        Image sobelEdges = EdgeDetector::detectEdges(grayImg, "Sobel");
        sobelEdges.saveToFile("sample_images/sobel_edges.png");
        
        // Test Prewitt edge detection
        std::cout << "\nTesting Prewitt edge detection..." << std::endl;
        Image prewittEdges = EdgeDetector::detectEdges(img, "Prewitt");
        prewittEdges.saveToFile("sample_images/prewitt_edges.png");
        
        std::cout << "\n All image operations successful!" << std::endl;
        std::cout << "Grayscale result: sample_images/output_gray.png" << std::endl;
        std::cout << "Sobel edges: sample_images/sobel_edges.png" << std::endl;
        std::cout << "Prewitt edges: sample_images/prewitt_edges.png" << std::endl;

        std::cout << "\n All image operations successful!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Testing Image I/O Program Completed!" << std::endl;
    return 0;
}