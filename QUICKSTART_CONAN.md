# Quick Start Guide for Conan with geometry-central

This guide will get you up and running with geometry-central using Conan 2 in under 5 minutes.

## Install Conan 2

```bash
pip install "conan>=2.0"
conan profile detect --force
```

## Option A: Use geometry-central in Your Project

### 1. Add to your conanfile.txt:
```ini
[requires]
geometry-central/0.2.0

[generators]
CMakeDeps
CMakeToolchain
```

### 2. Or add to your conanfile.py:
```python
def requirements(self):
    self.requires("geometry-central/0.2.0")
```

### 3. Install and build:
```bash
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Option B: Build geometry-central from Source with Conan

```bash
# From the geometry-central repository root
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Option C: Create Local Conan Package

```bash
# From the geometry-central repository root
conan create . --build=missing
```

## Common Options

### Build as shared library:
```bash
conan install . -o geometry-central:shared=True --output-folder=build --build=missing
```

### Disable SuiteSparse:
```bash
conan install . -o geometry-central:with_suitesparse=False --output-folder=build --build=missing
```

### Debug build:
```bash
conan install . -s build_type=Debug --output-folder=build --build=missing
```

## Verify Installation

Run the consumer example:
```bash
cd examples/conan_consumer
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
./my_app
```

## Need Help?

- See [CONAN.md](CONAN.md) for detailed documentation
- See [TESTING_CONAN.md](TESTING_CONAN.md) for testing instructions
- Check the [example project](examples/conan_consumer/) for a complete working example

## Traditional Build (Without Conan)

The existing CMake-based build system continues to work without Conan:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Dependencies will be fetched automatically using CMake's FetchContent.
