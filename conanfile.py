from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy, load
from conan.tools.scm import Version
import os
import re


class GeometryCentralConan(ConanFile):
    name = "geometry-central"
    version = "0.2.0"
    license = "MIT"
    author = "Nicholas Sharp and contributors"
    url = "https://github.com/nmwsharp/geometry-central"
    homepage = "https://geometry-central.net"
    description = "A modern C++ library of data structures and algorithms for geometry processing"
    topics = ("geometry", "geometry-processing", "mesh", "cpp")
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_suitesparse": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "with_suitesparse": True,
    }
    
    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "deps/*", "cmake/*"
    
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
    
    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
    
    def layout(self):
        cmake_layout(self)
    
    def requirements(self):
        # Eigen is the main required dependency
        self.requires("eigen/3.4.0")
        
        # Optional dependencies
        if self.options.with_suitesparse:
            # SuiteSparse is optional but recommended for better sparse solvers
            # Note: SuiteSparse may not be available in ConanCenter
            # Users can disable with -o with_suitesparse=False
            pass  # SuiteSparse typically needs to be system-provided
    
    def build_requirements(self):
        # Build-only dependencies would go here
        pass
    
    def generate(self):
        # CMakeDeps generates Find*.cmake files for dependencies
        deps = CMakeDeps(self)
        deps.generate()
        
        # CMakeToolchain generates conan_toolchain.cmake
        tc = CMakeToolchain(self)
        
        # Pass options to CMake
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        
        # Control SuiteSparse detection
        if not self.options.with_suitesparse:
            tc.variables["SUITESPARSE"] = "OFF"
        
        tc.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
        
        # Copy license
        copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
    
    def package_info(self):
        # Library name
        self.cpp_info.libs = ["geometry-central"]
        
        # Set the include directory
        self.cpp_info.includedirs = ["include"]
        
        # Define compile definitions that consumers might need
        self.cpp_info.defines = ["NOMINMAX", "_USE_MATH_DEFINES"]
        
        # If built with SuiteSparse, consumers should know
        if self.options.with_suitesparse:
            self.cpp_info.defines.append("GC_HAVE_SUITESPARSE")
        
        # C++ standard requirement
        self.cpp_info.cxxflags = ["-std=c++11"]
        
        # Windows-specific settings
        if self.settings.os == "Windows":
            self.cpp_info.system_libs = []
