# Conan Consumer Example

This example demonstrates how to use geometry-central as a Conan dependency in your own project.

## Prerequisites

- Conan 2.x installed: `pip install "conan>=2.0"`
- CMake 3.15 or higher
- C++11 compatible compiler

## Steps to Build

1. First, make sure geometry-central is available in your Conan cache. From the root of the geometry-central repository:

```bash
conan create . --build=missing
```

2. Navigate to this example directory:

```bash
cd examples/conan_consumer
```

3. Install dependencies:

```bash
conan install . --output-folder=build --build=missing
```

4. Build the example:

```bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

5. Run the example:

```bash
./my_app  # On Linux/macOS
my_app.exe  # On Windows
```

## What This Example Shows

- How to declare geometry-central as a dependency in `conanfile.py`
- How to use CMake's `find_package()` with Conan-installed packages
- How to link against geometry-central in your CMakeLists.txt
- Basic usage of geometry-central functionality

## Project Structure

```
conan_consumer/
├── conanfile.py       # Conan dependency declaration
├── CMakeLists.txt     # CMake build configuration
├── main.cpp           # Example application code
└── README.md          # This file
```

## Customization

You can modify the `conanfile.py` to:
- Change build settings
- Add additional dependencies
- Configure geometry-central options (e.g., `-o geometry-central:shared=True`)

Example with custom options:
```bash
conan install . --output-folder=build -o geometry-central:shared=True --build=missing
```
