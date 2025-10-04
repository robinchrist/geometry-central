# Custom Managed Quantities - Example

This page provides a complete example of using custom managed quantities in geometry-central.

## Basic Example

Here's a simple example that computes vertex areas as a custom managed quantity:

```cpp
#include "geometrycentral/surface/vertex_position_geometry.h"
#include "geometrycentral/surface/meshio.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

int main() {
  // Load a mesh
  std::unique_ptr<SurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh("bunny.obj");

  // Create a custom quantity: vertex areas
  VertexData<double> customVertexAreas(*mesh);

  // Register it as a managed quantity with a compute function
  auto customQuantity = geometry->registerCustomManagedQuantity<VertexData<double>>(
      customVertexAreas,
      [&]() {
        // Compute function: compute vertex areas from face areas
        // IMPORTANT: Re-initialize the data buffer
        customVertexAreas = VertexData<double>(*mesh);
        
        geometry->requireFaceAreas();
        for (Vertex v : mesh->vertices()) {
          double area = 0.0;
          for (Face f : v.adjacentFaces()) {
            area += geometry->faceAreas[f] / f.degree();
          }
          customVertexAreas[v] = area;
        }
      });

  // Use the custom quantity
  customQuantity->require();  // Computes if needed

  // Access the data
  for (Vertex v : mesh->vertices()) {
    std::cout << "Vertex " << v << " has area " << customVertexAreas[v] << std::endl;
  }

  // Custom quantities work with refreshQuantities() and purgeQuantities()
  customQuantity->unrequire();
  geometry->purgeQuantities();

  return 0;
}
```

## Advanced Example: Multiple Custom Quantities

You can register multiple custom quantities, and they can depend on each other:

```cpp
#include "geometrycentral/surface/vertex_position_geometry.h"
#include "geometrycentral/surface/meshio.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

int main() {
  std::unique_ptr<SurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh("bunny.obj");

  // First custom quantity: edge midpoints
  EdgeData<Vector3> edgeMidpoints(*mesh);
  auto edgeMidpointsQ = geometry->registerCustomManagedQuantity<EdgeData<Vector3>>(
      edgeMidpoints,
      [&]() {
        edgeMidpoints = EdgeData<Vector3>(*mesh);
        geometry->requireVertexPositions();
        for (Edge e : mesh->edges()) {
          Halfedge he = e.halfedge();
          Vector3 p1 = geometry->vertexPositions[he.vertex()];
          Vector3 p2 = geometry->vertexPositions[he.twin().vertex()];
          edgeMidpoints[e] = 0.5 * (p1 + p2);
        }
      });

  // Second custom quantity: face centroids (depends on edge midpoints)
  FaceData<Vector3> faceCentroids(*mesh);
  auto faceCentroidsQ = geometry->registerCustomManagedQuantity<FaceData<Vector3>>(
      faceCentroids,
      [&]() {
        faceCentroids = FaceData<Vector3>(*mesh);
        geometry->requireVertexPositions();
        for (Face f : mesh->faces()) {
          Vector3 centroid = Vector3::zero();
          int count = 0;
          for (Vertex v : f.adjacentVertices()) {
            centroid += geometry->vertexPositions[v];
            count++;
          }
          faceCentroids[f] = centroid / count;
        }
      });

  // Use both quantities
  edgeMidpointsQ->require();
  faceCentroidsQ->require();

  // Do something with the data
  for (Face f : mesh->faces()) {
    std::cout << "Face centroid: " << faceCentroids[f] << std::endl;
  }

  // Cleanup
  edgeMidpointsQ->unrequire();
  faceCentroidsQ->unrequire();
  geometry->purgeQuantities();

  return 0;
}
```

## Important Notes

1. **Re-initialize the data buffer**: Your compute function should always re-initialize the data buffer at the beginning, like `customVertexAreas = VertexData<double>(*mesh);`. This is necessary because the buffer may be cleared after purging.

2. **Dependencies**: You can depend on built-in quantities (like `faceAreas`) or other custom quantities. Just call `require()` on them in your compute function.

3. **Automatic integration**: Custom quantities automatically work with:
   - `refreshQuantities()` - recomputes them when the underlying geometry changes
   - `purgeQuantities()` - clears them when they're unrequired

4. **Lifetime**: The returned handle (e.g., `customQuantity`) should be kept alive as long as you want to use the quantity.

5. **Access pattern**: You access the data directly through the original buffer (e.g., `customVertexAreas[v]`), not through `customQuantity->get()[v]` (though both work).
