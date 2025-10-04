#include <iostream>
#include "geometrycentral/utilities/vector3.h"

int main() {
    // Simple test to verify we can use geometry-central
    geometrycentral::Vector3 v(1.0, 2.0, 3.0);
    std::cout << "geometry-central test_package: Vector3 created successfully!" << std::endl;
    std::cout << "Vector length: " << v.norm() << std::endl;
    return 0;
}
