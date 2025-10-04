# Testing the Conan Integration

This document describes how to test and validate the Conan 2 integration for geometry-central.

## Quick Test Script

A convenience script `test_conan.sh` is provided in the root directory that automates the testing process:

```bash
./test_conan.sh
```

This script will:
1. Verify Conan 2.x is installed
2. Build geometry-central with Conan-managed dependencies
3. Create and validate the Conan package

## Manual Testing Steps

### 1. Prerequisites

Install Conan 2.x:
```bash
pip install "conan>=2.0"
```

Verify installation:
```bash
conan --version  # Should show 2.x
```

Create a default profile:
```bash
conan profile detect --force
```

### 2. Test Building with Conan Dependencies

From the repository root:

```bash
# Install dependencies
conan install . --output-folder=build --build=missing

# Configure and build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

**Expected result**: Library builds successfully with Eigen provided by Conan.

### 3. Test Creating a Conan Package

From the repository root:

```bash
conan create . --build=missing
```

This will:
- Export the recipe
- Install dependencies (Eigen from ConanCenter)
- Build the library
- Package the library
- Run the test_package validation

**Expected result**: 
- Build completes successfully
- test_package compiles and runs
- Package is stored in your local Conan cache

### 4. Test Using as a Dependency

Navigate to the consumer example:

```bash
cd examples/conan_consumer
```

Build and run:

```bash
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
./my_app  # or my_app.exe on Windows
```

**Expected result**: Example application compiles, links, and runs successfully.

## Testing Different Configurations

### Test with Shared Library

```bash
conan create . --build=missing -o shared=True
```

### Test without SuiteSparse

```bash
conan create . --build=missing -o with_suitesparse=False
```

### Test with Different Build Types

```bash
conan create . --build=missing -s build_type=Debug
conan create . --build=missing -s build_type=Release
```

### Test with Different Compilers

On Linux with both gcc and clang:

```bash
# GCC
conan create . --build=missing -s compiler=gcc -s compiler.version=11

# Clang
conan create . --build=missing -s compiler=clang -s compiler.version=14
```

## Validation Checklist

- [ ] Conan 2.x is properly installed
- [ ] Default profile is created and detected
- [ ] Dependencies (Eigen) are resolved from ConanCenter
- [ ] Library builds successfully with Conan toolchain
- [ ] `conan create` completes without errors
- [ ] test_package compiles and runs
- [ ] Package can be consumed by other projects
- [ ] Consumer example builds and runs
- [ ] Both shared and static builds work
- [ ] Different build types (Debug/Release) work
- [ ] Traditional CMake build (without Conan) still works

## Troubleshooting

### "CMake toolchain file not found"

Make sure you're using the Conan-generated toolchain:
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
```

### "Package 'Eigen3' not found"

Ensure dependencies are installed:
```bash
conan install . --output-folder=build --build=missing
```

### "Wrong Conan version"

Upgrade to Conan 2.x:
```bash
pip install --upgrade "conan>=2.0"
```

### Build failures with SuiteSparse

SuiteSparse is optional and often needs system installation. Disable it if not needed:
```bash
conan create . -o with_suitesparse=False --build=missing
```

### Comparing with Traditional Build

To verify backward compatibility, test the traditional build:

```bash
# Clean build
rm -rf build && mkdir build && cd build

# Configure without Conan
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build .
```

This should work independently of Conan, using FetchContent for Eigen if needed.

## CI/CD Integration

For continuous integration, add steps like:

```yaml
- name: Install Conan
  run: pip install "conan>=2.0"

- name: Create Conan profile
  run: conan profile detect --force

- name: Test Conan package
  run: conan create . --build=missing
```

## Performance Notes

First-time builds will be slower as Conan downloads and builds dependencies. Subsequent builds use cached packages and are much faster.

To speed up testing, use `--build=missing` to only build what's not in the cache, rather than `--build=*` which rebuilds everything.
