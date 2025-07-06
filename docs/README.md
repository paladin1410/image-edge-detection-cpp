# Edge Detection Backend in C++

A C++ console application that performs image edge detection using Sobel or Prewitt operators based on user input.

## Quick Start

### Clone and Build
```bash
# Clone the repository
git clone https://github.com/paladin1410/image-edge-detection-cpp.git

# Navigate to project directory
cd image-edge-detection-cpp

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
make

# Return to main directory for usage
cd ..
```

### Run Edge Detection
```bash
# Choose Sobel operator
./build/edge_detector sample_images/cameraman.jpg Sobel

# Choose Prewitt operator  
./build/edge_detector sample_images/coconut_gray.png Prewitt

# Case-insensitive operators supported
./build/edge_detector sample_images/lenna.png sobel
```

### Run Test Suite
```bash
# From main directory
./build/tests
```

## Usage

```bash
# From the main project directory
./build/edge_detector <image_path> <operator>

Arguments:
  image_path    Path to input image (PNG, JPG, etc.)
  operator      Edge detection operator: Sobel, Prewitt (case-insensitive)

Examples:
  ./build/edge_detector sample_images/cameraman.jpg Sobel
  ./build/edge_detector sample_images/test.png Prewitt
```

Results are saved to the `output` folder as `result_<operator>_edges.png`.

## Project Structure

```
image-edge-detection-cpp/
├── src/                   # Source files
│   ├── main.cpp           # Main program
│   ├── Image.cpp          # Image loading/saving/processing
│   └── EdgeDetector.cpp   # Edge detection algorithms
├── include/               # Header files
│   ├── Image.h            # Image class declaration
│   └── EdgeDetector.h     # EdgeDetector class declaration
├── tests/                 # Unit and integration tests
│   └── test_suite.cpp     # Comprehensive test suite
├── sample_images/         # Input test images
├── output/                # Generated edge detection results
├── docs/                  # Documentation and diagrams
│   ├── README.md          # Project documentation
│   ├── edge_detector_architecture.puml  # UML source file
│   └── edge_detector_architecture.png   # UML class diagram
├── third_party/          # External libraries (STB headers)
└── CMakeLists.txt        # Build configuration
```

## How It Works

This program detects edges in images using two different algorithms:
- **Sobel** - Uses Sobel operators for gradient calculation
- **Prewitt** - Uses Prewitt operators for gradient calculation

The program automatically converts color images to grayscale before edge detection and handles various image formats (PNG, JPG, etc.).

## Architecture

See the [class diagram](edge_detector_architecture.png) showing how the edge detection algorithms are organized.

**Key Components:**
- `Image` class - Handles image loading, saving, and grayscale conversion
- `EdgeDetector` class - Implements Sobel and Prewitt edge detection algorithms

## Requirements

- C++17 compatible compiler
- CMake 3.16 or higher
- Git 
- STB image library (included as headers)

## Testing Suite

The project includes comprehensive unit and integration tests.
- Image class functionality (loading, saving, conversion)
- EdgeDetector class functionality
- Complete edge detection workflows
- Error handling and edge cases