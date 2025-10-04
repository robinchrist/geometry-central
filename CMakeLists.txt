cmake_minimum_required(VERSION 3.14.0)

project(geometry-central)

### Policy settings
cmake_policy(SET CMP0054 NEW)   # don't implicitly dereference inside if()

# Conan 2 support: include the toolchain if it exists
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
  include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

### Process settings
option(BUILD_SHARED_LIBS "Build the shared library" FALSE)
if(BUILD_SHARED_LIBS)
    message("-- Building SHARED libraries")
else()
    message("-- Building STATIC libraries")
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake") # look for stuff in the /cmake directory
include(UpdateCacheVariable)

# Work with non-standard homebrew installations
# (from ceres build system)
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  find_program(HOMEBREW_EXECUTABLE brew)
  mark_as_advanced(FORCE HOMEBREW_EXECUTABLE)
  if (HOMEBREW_EXECUTABLE)
    # Detected a Homebrew install, query for its install prefix.
    execute_process(COMMAND ${HOMEBREW_EXECUTABLE} --prefix
      OUTPUT_VARIABLE HOMEBREW_INSTALL_PREFIX
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Detected Homebrew with install prefix: "
      "${HOMEBREW_INSTALL_PREFIX}, adding to CMake search paths.")
    list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_INSTALL_PREFIX}")
  endif()
endif()


### Handle windows-specific fixes
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions (-DNOMINMAX)  # don't use weird windows built-in min/max
  add_definitions (-D_USE_MATH_DEFINES)  # match unix behavior of constants in cmath
endif()

### Do anything needed for dependencies and bring their stuff in to scope
add_subdirectory(deps)

# copy variables set by deps upward
SET(GC_HAVE_SUITESPARSE ${GC_HAVE_SUITESPARSE} PARENT_SCOPE)

### Recurse to the source code
add_subdirectory(src)

# Modern CMake package configuration for Conan and general use
include(CMakePackageConfigHelpers)

# install
install(
  TARGETS geometry-central
  EXPORT geometry-centralTargets
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.ipp")

# Install CMake config files for find_package() support
install(EXPORT geometry-centralTargets
  FILE geometry-centralTargets.cmake
  NAMESPACE geometry-central::
  DESTINATION lib/cmake/geometry-central)

# Create and install config file
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/geometry-centralConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/geometry-centralConfig.cmake"
  INSTALL_DESTINATION lib/cmake/geometry-central)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/geometry-centralConfigVersion.cmake"
  VERSION 0.2.0
  COMPATIBILITY SameMajorVersion)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/geometry-centralConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/geometry-centralConfigVersion.cmake"
  DESTINATION lib/cmake/geometry-central)
