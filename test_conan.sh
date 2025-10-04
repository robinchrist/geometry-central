#!/bin/bash
# This script demonstrates how to use Conan with geometry-central
# Note: This requires Conan 2.x to be installed (pip install "conan>=2.0")

set -e

echo "===== Conan Integration Test for geometry-central ====="
echo ""

# Check if Conan is installed
if ! command -v conan &> /dev/null; then
    echo "ERROR: Conan is not installed"
    echo "Please install Conan 2.x: pip install 'conan>=2.0'"
    exit 1
fi

# Check Conan version
CONAN_VERSION=$(conan --version | grep -oP '\d+\.\d+' | head -1)
MAJOR_VERSION=$(echo $CONAN_VERSION | cut -d. -f1)

if [ "$MAJOR_VERSION" -lt 2 ]; then
    echo "ERROR: Conan 1.x detected. Please upgrade to Conan 2.x"
    echo "Run: pip install --upgrade 'conan>=2.0'"
    exit 1
fi

echo "✓ Conan $CONAN_VERSION detected"
echo ""

# Detect default profile if not exists
if [ ! -d ~/.conan2/profiles ]; then
    echo "Creating default Conan profile..."
    conan profile detect --force
    echo ""
fi

echo "===== Method 1: Build with Conan managing dependencies ====="
echo ""

# Install dependencies
echo "Step 1: Installing dependencies via Conan..."
conan install . --output-folder=build --build=missing -s build_type=Release

echo ""
echo "Step 2: Building with CMake using Conan toolchain..."
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .

cd ..
echo ""
echo "✓ Build completed successfully!"
echo ""

echo "===== Method 2: Create Conan package ====="
echo ""

# This creates and tests the Conan package
echo "Creating Conan package..."
conan create . --build=missing

echo ""
echo "✓ Conan package created and tested successfully!"
echo ""

echo "===== Summary ====="
echo "✓ Dependencies installed via Conan"
echo "✓ Library built with Conan toolchain"
echo "✓ Conan package created and validated"
echo ""
echo "You can now use geometry-central in your projects by adding:"
echo "  requires = \"geometry-central/0.2.0\""
echo "to your conanfile.py or conanfile.txt"
