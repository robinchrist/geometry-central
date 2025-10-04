from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class ConsumerConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # Use geometry-central from Conan
        self.requires("geometry-central/0.2.0")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
