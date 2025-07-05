/**
 * Test Suite for Edge Detection Assignment
 * 
 * Tests the core assignment requirements:
 * 1. Image class functionality
 * 2. EdgeDetector class functionality
 * 3. Integration test
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#include <stdexcept>
#include "../include/Image.h"
#include "../include/EdgeDetector.h"


//Test framework
void runTest(const std::string& testName, bool (*testFunc)()) {
    std::cout << "Running: " << testName << "... ";
    try {
        if (testFunc()) {
            std::cout << "✅ PASSED" << std::endl;
        } else {
            std::cout << "❌ FAILED" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "❌ FAILED (Exception: " << e.what() << ")" << std::endl;
    }
}

// =============================================================================
// UNIT TESTS - IMAGE CLASS (Loading/Saving)
// =============================================================================

// A. Constructor Tests
bool test_image_constructor_valid_data() {
    // Test: Create an Image with valid data and verify properties
    std::vector<uint8_t> testData = {
        0, 128, 255,
        64, 192, 32,
        200, 100, 50
    };
    
    Image image(testData, 3, 3, 1);
    
    // Verify dimensions and data
    return image.getWidth() == 3 && 
           image.getHeight() == 3 && 
           image.getChannels() == 1 &&
           image.getData().size() == 9;
}

bool test_image_constructor_invalid_dimensions() {
    // Test: Constructor should throw with invalid dimensions
    try {
        std::vector<uint8_t> data = {1, 2, 3, 4};
        Image image(data, 0, 2, 1); // Zero width
        return false; // Should have thrown
    } catch (const std::invalid_argument&) {
        return true; // Expected
    } catch (...) {
        return false; // Wrong exception type
    }
}

bool test_image_constructor_data_size_mismatch() {
    // Test: Constructor should throw when data size doesn't match dimensions
    try {
        std::vector<uint8_t> data = {1, 2, 3}; // 3 elements
        Image image(data, 3, 3, 1); // But expecting 3*3*1 = 9 elements
        return false; // Should have thrown
    } catch (const std::invalid_argument&) {
        return true; // Expected
    } catch (...) {
        return false; // Wrong exception type
    }
}

// B. File Loading Tests
bool test_image_load_nonexistent_file() {
    // Test: Loading non-existent file should throw
    try {
        Image::loadFromFile("nonexistent_file.png");
        return false; // Should have thrown
    } catch (const std::runtime_error&) {
        return true; // Expected
    } catch (...) {
        return false; // Wrong exception type
    }
}

bool test_image_load_empty_filepath() {
    // Test: Empty filepath should throw
    try {
        Image::loadFromFile("");
        return false; // Should have thrown
    } catch (const std::invalid_argument&) {
        return true; // Expected
    } catch (...) {
        return false; // Wrong exception type
    }
}

// C. File Saving Tests  
bool test_image_save_empty_filepath() {
    // Test: Saving with empty filepath should throw
    std::vector<uint8_t> data = {128, 128, 128, 128, 128, 128, 128, 128, 128};
    Image image(data, 3, 3, 1);
    
    try {
        image.saveToFile("");
        return false; // Should have thrown
    } catch (const std::invalid_argument&) {
        return true; // Expected
    } catch (...) {
        return false; // Wrong exception type
    }
}

bool test_image_save_invalid_image_data() {
    // Test: Saving image with no data should throw
    try {
        std::vector<uint8_t> emptyData;
        Image invalidImage(emptyData, 0, 0, 1); 
        return false; // Should have thrown
    } catch (const std::invalid_argument&) {
        return true; // Expected - constructor should catch this
    } catch (...) {
        return false; // Wrong exception type
    }
}

// =============================================================================
// EDGEDETECTOR CLASS TEST  
// =============================================================================

bool test_edge_detector_operator_selection() {
    // Test: Verify that different operators can be selected and produce results
    std::vector<uint8_t> testData = {
        0, 128, 255,
        0, 128, 255,
        0, 128, 255
    };
    Image testImage(testData, 3, 3, 1);
    
    // Test Sobel operator selection
    Image sobelResult = EdgeDetector::detectEdges(testImage, "Sobel");
    
    // Test Prewitt operator selection  
    Image prewittResult = EdgeDetector::detectEdges(testImage, "Prewitt");
    
    // Verify both operators work and produce valid results
    return sobelResult.getWidth() == 3 && sobelResult.getHeight() == 3 &&
           prewittResult.getWidth() == 3 && prewittResult.getHeight() == 3;
}

bool test_edge_detector_invalid_operator() {
    // Test: Invalid operator should throw exception
    std::vector<uint8_t> data(9, 128);
    Image testImage(data, 3, 3, 1);
    
    try {
        EdgeDetector::detectEdges(testImage, "InvalidOperator");
        return false; // Should have thrown
    } catch (const std::invalid_argument&) {
        return true; // Expected
    } catch (...) {
        return false; // Wrong exception type
    }
}

bool test_edge_detector_case_sensitivity() {
    // Test: Operator names are case-sensitive
    std::vector<uint8_t> data(9, 128);
    Image testImage(data, 3, 3, 1);
    
    try {
        EdgeDetector::detectEdges(testImage, "sobel"); // lowercase
        return false; // Should have thrown
    } catch (const std::invalid_argument&) {
        return true; // Expected - case sensitive
    } catch (...) {
        return false; // Wrong exception type
    }
}

bool test_edge_detector_too_small_image() {
    // Test: Image smaller than 3x3 should throw
    std::vector<uint8_t> tinyData = {255, 128, 64, 32}; // 2x2 image
    Image tinyImage(tinyData, 2, 2, 1);
    
    try {
        EdgeDetector::detectEdges(tinyImage, "Sobel");
        return false; // Should have thrown
    } catch (const std::runtime_error&) {
        return true; // Expected
    } catch (...) {
        return false; // Wrong exception type
    }
}

bool test_edge_detector_rgb_conversion() {
    // Test: RGB image should be automatically converted to grayscale
    std::vector<uint8_t> rgbData = {
        255, 0, 0,   0, 255, 0,   0, 0, 255,    
        255, 255, 0, 255, 0, 255, 0, 255, 255,  
        128, 128, 128, 64, 64, 64, 192, 192, 192 
    };
    Image rgbImage(rgbData, 3, 3, 3); 
    
    Image result = EdgeDetector::detectEdges(rgbImage, "Sobel");
    
    // Result should be grayscale (1 channel) with same dimensions
    return result.getChannels() == 1 && 
           result.getWidth() == 3 && 
           result.getHeight() == 3;
}

bool test_edge_detector_different_operators_produce_different_results() {
    // Test: Sobel and Prewitt should produce different results
    std::vector<uint8_t> testData = {
        1, 2, 3, 4, 5,
        1, 2, 3, 4, 5,
        1, 2, 3, 4, 5,
        1, 2, 3, 4, 5,
        1, 2, 3, 4, 5
    };
    Image testImage(testData, 5, 5, 1);
    
    std::cout << "\n  DEBUG: Input image (5x5):" << std::endl;
    std::cout << "  ";
    for (int i = 0; i < 25; ++i) {
        if (i > 0 && i % 5 == 0) std::cout << "\n  ";
        std::cout << std::setw(3) << (int)testData[i] << " ";
    }
    std::cout << std::endl;
    
    Image sobelResult = EdgeDetector::detectEdges(testImage, "Sobel");
    Image prewittResult = EdgeDetector::detectEdges(testImage, "Prewitt");
    
    const auto& sobelData = sobelResult.getData();
    const auto& prewittData = prewittResult.getData();
    
    std::cout << "  DEBUG: Sobel result:" << std::endl;
    std::cout << "  ";
    for (int i = 0; i < 25; ++i) {
        if (i > 0 && i % 5 == 0) std::cout << "\n  ";
        std::cout << std::setw(3) << (int)sobelData[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "  DEBUG: Prewitt result:" << std::endl;
    std::cout << "  ";
    for (int i = 0; i < 25; ++i) {
        if (i > 0 && i % 5 == 0) std::cout << "\n  ";
        std::cout << std::setw(3) << (int)prewittData[i] << " ";
    }
    std::cout << std::endl;
    
    // Check for differences
    int differences = 0;
    for (size_t i = 0; i < sobelData.size(); ++i) {
        if (sobelData[i] != prewittData[i]) {
            differences++;
        }
    }
    
    std::cout << "  DEBUG: Found " << differences << " different pixels out of " << sobelData.size() << std::endl;
    
    // Results should be different for at least some pixels
    return differences > 0;
}

bool test_edge_detector_uniform_image() {
    // Test: Uniform image should produce minimal edges
    std::vector<uint8_t> uniformData(25, 128); 
    Image uniformImage(uniformData, 5, 5, 1);
    
    Image result = EdgeDetector::detectEdges(uniformImage, "Sobel");
    const auto& resultData = result.getData();
    
    // Most pixels should be near zero (no edges in uniform image)
    int lowValuePixels = 0;
    for (uint8_t pixel : resultData) {
        if (pixel < 50) {
            lowValuePixels++;
        }
    }
    
    // Expect most pixels to be low (at least 20 out of 25)
    return lowValuePixels >= 20;
}

// =============================================================================
// INTEGRATION TESTS - FULL PIPELINE/WORKFLOW  
// =============================================================================

bool test_integration_create_process_save_pipeline() {
    // Test: Create image → Edge detection → Save → Load → Verify
    try {
        // Step 1: Create a test image with clear edges
        std::vector<uint8_t> testData = {
            0,   0,   0,   255, 255,
            0,   0,   0,   255, 255,
            0,   0,   0,   255, 255,
            255, 255, 255, 255, 255,
            255, 255, 255, 255, 255
        };
        Image originalImage(testData, 5, 5, 1);
        
        // Step 2: Apply edge detection
        Image edgeResult = EdgeDetector::detectEdges(originalImage, "Sobel");
        
        // Step 3: Save result to file
        edgeResult.saveToFile("test_integration_result.png");
        
        // Step 4: Load the saved file back
        Image loadedResult = Image::loadFromFile("test_integration_result.png");
        
        // Step 5: Verify the loaded image has correct properties
        bool success = loadedResult.getWidth() == 5 && 
                      loadedResult.getHeight() == 5 && 
                      loadedResult.getChannels() == 1;
        
        // Cleanup
        std::remove("test_integration_result.png");
        
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "\n  Integration test failed: " << e.what() << std::endl;
        return false;
    }
}

bool test_integration_rgb_to_grayscale_pipeline() {
    // Test: RGB image → Grayscale conversion → Edge detection
    try {
        // Step 1: Create RGB test image
        std::vector<uint8_t> rgbData = {
            255, 0, 0,   0, 255, 0,   0, 0, 255,      
            255, 255, 0, 255, 0, 255, 0, 255, 255,     
            128, 128, 128, 64, 64, 64, 192, 192, 192  
        };
        Image rgbImage(rgbData, 3, 3, 3); // 3 channels
        
        // Step 2: Convert to grayscale manually first
        Image grayscaleImage = rgbImage.toGrayscale();
        
        // Step 3: Apply edge detection (should work on grayscale)
        Image edgeResult1 = EdgeDetector::detectEdges(grayscaleImage, "Sobel");
        
        // Step 4: Apply edge detection directly on RGB (should convert internally)
        Image edgeResult2 = EdgeDetector::detectEdges(rgbImage, "Prewitt");
        
        // Step 5: Verify both approaches produce valid results
        return edgeResult1.getChannels() == 1 && edgeResult1.getWidth() == 3 &&
               edgeResult2.getChannels() == 1 && edgeResult2.getWidth() == 3;
        
    } catch (const std::exception& e) {
        std::cout << "\n  RGB pipeline test failed: " << e.what() << std::endl;
        return false;
    }
}

bool test_integration_both_operators_complete_workflow() {
    // Test: Complete workflow with both Sobel and Prewitt operators
    try {
        // Step 1: Create test image
        std::vector<uint8_t> testData = {
            0, 128, 255,
            0, 128, 255,
            0, 128, 255
        };
        Image testImage(testData, 3, 3, 1);
        
        // Step 2: Process with Sobel
        Image sobelResult = EdgeDetector::detectEdges(testImage, "Sobel");
        sobelResult.saveToFile("test_sobel_output.png");
        
        // Step 3: Process with Prewitt  
        Image prewittResult = EdgeDetector::detectEdges(testImage, "Prewitt");
        prewittResult.saveToFile("test_prewitt_output.png");
        
        // Step 4: Verify both files can be loaded back
        Image loadedSobel = Image::loadFromFile("test_sobel_output.png");
        Image loadedPrewitt = Image::loadFromFile("test_prewitt_output.png");
        
        // Step 5: Verify properties
        bool success = loadedSobel.getWidth() == 3 && loadedSobel.getHeight() == 3 &&
                      loadedPrewitt.getWidth() == 3 && loadedPrewitt.getHeight() == 3;
        
        // Cleanup
        std::remove("test_sobel_output.png");
        std::remove("test_prewitt_output.png");
        
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "\n  Both operators workflow test failed: " << e.what() << std::endl;
        return false;
    }
}


// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main() {
    std::cout << "Edge Detection Test Suite" << std::endl;
    std::cout << "=========================" << std::endl;
    
    // UNIT TESTS - IMAGE CLASS
    std::cout << "\n--- IMAGE CLASS UNIT TESTS ---" << std::endl;
    
    // Constructor tests
    runTest("Image Constructor with Valid Data", test_image_constructor_valid_data);
    runTest("Image Constructor with Invalid Dimensions", test_image_constructor_invalid_dimensions);
    runTest("Image Constructor with Data Size Mismatch", test_image_constructor_data_size_mismatch);
    
    // File loading tests
    runTest("Image Load Nonexistent File", test_image_load_nonexistent_file);
    runTest("Image Load Empty Filepath", test_image_load_empty_filepath);
    
    // File saving tests
    runTest("Image Save Empty Filepath", test_image_save_empty_filepath);
    runTest("Image Save Invalid Image Data", test_image_save_invalid_image_data);
    
    // UNIT TESTS - EDGEDETECTOR CLASS
    std::cout << "\n--- EDGEDETECTOR CLASS UNIT TESTS ---" << std::endl;
    runTest("EdgeDetector Operator Selection", test_edge_detector_operator_selection);
    runTest("EdgeDetector Invalid Operator", test_edge_detector_invalid_operator);
    runTest("EdgeDetector Case Sensitivity", test_edge_detector_case_sensitivity);
    runTest("EdgeDetector Too Small Image", test_edge_detector_too_small_image);
    runTest("EdgeDetector RGB Conversion", test_edge_detector_rgb_conversion);
    runTest("EdgeDetector Different Operators Produce Different Results", test_edge_detector_different_operators_produce_different_results);
    runTest("EdgeDetector Uniform Image", test_edge_detector_uniform_image);
    
    // INTEGRATION TESTS - COMPLETE WORKFLOWS
    std::cout << "\n--- INTEGRATION TESTS (FULL PIPELINE) ---" << std::endl;
    runTest("Integration: Create→Process→Save→Load Pipeline", test_integration_create_process_save_pipeline);
    runTest("Integration: RGB→Grayscale→EdgeDetect Pipeline", test_integration_rgb_to_grayscale_pipeline);
    runTest("Integration: Both Operators Complete Workflow", test_integration_both_operators_complete_workflow);
    
    std::cout << "\n Tests completed!" << std::endl;
    std::cout << "Assignment requirements tested:" << std::endl;
    std::cout << "  • Image class functionality (constructor, loading, saving)" << std::endl;
    std::cout << "  • Operator selection logic (Sobel/Prewitt)" << std::endl;
    std::cout << "  • Edge detection error handling" << std::endl;
    std::cout << "  • RGB to grayscale conversion" << std::endl;
    std::cout << "  • Complete end-to-end workflows" << std::endl;
    
    return 0;
}