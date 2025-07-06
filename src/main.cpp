#include <iostream>       
#include <exception>
#include <filesystem>      
#include "Image.h"        
#include "EdgeDetector.h"

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <image_path> <operator>" << std::endl;
        std::cout << "Operators: Sobel, Prewitt (case-insensitive)" << std::endl;
        std::cout << "Example: " << argv[0] << " sample_images/cameraman.jpg Sobel" << std::endl;
        return 1;
    }
    
    std::string imagePath = argv[1];
    std::string operatorName = argv[2];
    
    std::cout << "Edge Detection Program" << std::endl;
    std::cout << "======================" << std::endl;
    std::cout << "Input image: " << imagePath << std::endl;
    std::cout << "Edge detection operator: " << operatorName << std::endl;
    
    try {
        // Load the image
        std::cout << "\nLoading image..." << std::endl;
        Image img = Image::loadFromFile(imagePath);
        std::cout << "Image loaded successfully: " << img.getWidth() << "x" << img.getHeight() 
                  << " (" << img.getChannels() << " channels)" << std::endl;
        
        // Apply edge detection with user's chosen operator
        std::cout << "\nApplying " << operatorName << " edge detection..." << std::endl;
        Image edgeResult = EdgeDetector::detectEdges(img, operatorName);
        
        // Create output directory if it doesn't exist
        std::string outputDir = "output";
        std::filesystem::create_directories(outputDir);

        // Generate output filename in the output folder
        std::string outputPath = outputDir + "/result_" + operatorName + "_edges.png";

        // Save the result
        std::cout << "\nSaving result..." << std::endl;
        edgeResult.saveToFile(outputPath);
        
        std::cout << "\n😊 Edge detection completed successfully!" << std::endl;
        std::cout << "Result saved to: " << outputPath << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}