#include <iostream>
#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/utilities/vector3.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

int main() {
    std::cout << "geometry-central consumer example" << std::endl;
    
    // Create a simple vector to test the library is linked
    Vector3 v1(1.0, 2.0, 3.0);
    Vector3 v2(4.0, 5.0, 6.0);
    Vector3 v3 = v1 + v2;
    
    std::cout << "Vector sum: (" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;
    std::cout << "Vector norm: " << v3.norm() << std::endl;
    
    std::cout << "geometry-central is working correctly!" << std::endl;
    
    return 0;
}
