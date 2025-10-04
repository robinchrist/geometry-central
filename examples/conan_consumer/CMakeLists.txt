cmake_minimum_required(VERSION 3.15)
project(geometry_central_consumer CXX)

# Find geometry-central package installed by Conan
find_package(geometry-central REQUIRED CONFIG)

# Create an executable that uses geometry-central
add_executable(my_app main.cpp)
target_link_libraries(my_app geometry-central::geometry-central)
target_compile_features(my_app PUBLIC cxx_std_11)
