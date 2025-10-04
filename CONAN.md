# Conan Package Manager Integration

This document describes how to use Conan 2 for dependency management with geometry-central.

## What is Conan?

[Conan](https://conan.io/) is a modern C/C++ package manager that simplifies dependency management. This project supports Conan 2, which brings significant improvements and follows current best practices.

## Prerequisites

Install Conan 2.x (not Conan 1.x):

```bash
pip install "conan>=2.0"
```

Verify installation:

```bash
conan --version
```

## Using geometry-central as a Conan Package

### As a Consumer

If you want to use geometry-central in your project via Conan:

1. Add geometry-central to your `conanfile.txt`:

```ini
[requires]
geometry-central/0.2.0

[generators]
CMakeDeps
CMakeToolchain
```

2. Or in your `conanfile.py`:

```python
from conan import ConanFile

class MyProjectConan(ConanFile):
    requires = "geometry-central/0.2.0"
    generators = "CMakeDeps", "CMakeToolchain"
```

3. Install dependencies:

```bash
conan install . --output-folder=build --build=missing
```

4. Configure and build your project:

```bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Building from Source with Conan

To build geometry-central itself using Conan for dependency management:

1. Install dependencies via Conan:

```bash
conan install . --output-folder=build --build=missing
```

2. Configure CMake with Conan toolchain:

```bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```

3. Build:

```bash
cmake --build .
```

## Configuration Options

The Conan package supports several options:

- `shared`: Build as shared library (default: False)
- `with_suitesparse`: Enable SuiteSparse support (default: True)

Example with custom options:

```bash
conan install . --output-folder=build -o shared=True -o with_suitesparse=False --build=missing
```

## Creating a Conan Package

To create a local Conan package:

```bash
conan create . --build=missing
```

This will:
1. Export the recipe
2. Install dependencies
3. Build the package
4. Run the test_package validation
5. Store the package in your local Conan cache

## Conan Profiles

Conan uses profiles to define build settings. Detect your default profile:

```bash
conan profile detect --force
```

View your profile:

```bash
conan profile show
```

## Integration with Existing Build System

geometry-central maintains backward compatibility with its existing CMake-based build system. The Conan integration is optional:

- **With Conan**: Dependencies are managed by Conan, providing reproducible builds
- **Without Conan**: The existing FetchContent-based system continues to work

The CMake configuration automatically detects if Conan is being used by checking for `conan_toolchain.cmake`.

## Dependencies

The package declares the following dependencies:

- **Eigen** (required): Linear algebra library, version 3.4.0
- **SuiteSparse** (optional): Sparse matrix solvers - typically system-provided

Note: SuiteSparse is not available in ConanCenter, so when `with_suitesparse=True`, you need to have it installed on your system.

## Best Practices

1. **Use Conan 2.x**: This package follows Conan 2 best practices and may not work with Conan 1.x
2. **Version Pinning**: Pin specific versions in production to ensure reproducibility
3. **Build from Source**: Use `--build=missing` to build dependencies from source when binaries aren't available
4. **Profiles**: Use custom profiles for cross-compilation or special build requirements

## Troubleshooting

### CMake can't find dependencies

Make sure you're using the Conan-generated toolchain:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
```

### Build failures

Try building all dependencies from source:

```bash
conan install . --output-folder=build --build=missing
```

### SuiteSparse not found

If you want to use SuiteSparse, make sure it's installed on your system. Otherwise, disable it:

```bash
conan install . --output-folder=build -o with_suitesparse=False --build=missing
```

## Further Reading

- [Conan Documentation](https://docs.conan.io/)
- [Conan 2.0 Migration Guide](https://docs.conan.io/2/tutorial.html)
- [geometry-central Documentation](https://geometry-central.net)
