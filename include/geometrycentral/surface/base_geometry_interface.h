#pragma once

#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/utilities/dependent_quantity.h"
#include "geometrycentral/utilities/vector2.h"
#include "geometrycentral/utilities/vector3.h"

#include <memory>
#include <functional>

namespace geometrycentral {
namespace surface {

class BaseGeometryInterface {

public:
  BaseGeometryInterface(SurfaceMesh& mesh);
  virtual ~BaseGeometryInterface();

  // == Members
  SurfaceMesh& mesh;


  // == Utility methods

  // Recompute all require'd quantities from input data. Call this after e.g. repositioning a vertex or mutating the
  // mesh
  void refreshQuantities();

  // Clear out any cached quantities which were previously computed but are not currently required.
  void purgeQuantities();

  // Construct a geometry object on another mesh identical to this one
  // TODO move this to exist in realizations only
  std::unique_ptr<BaseGeometryInterface> reinterpretTo(SurfaceMesh& targetMesh);

  // == Custom managed quantities
  // Allows users to register custom quantities that work similarly to built-in managed quantities

  // Helper class for managing custom quantities
  template <typename T>
  class CustomManagedQuantity {
  public:
    CustomManagedQuantity(T* dataBuffer_, std::function<void()> computeFunc_, 
                          std::vector<DependentQuantity*>& quantities_)
        : quantityD(dataBuffer_, computeFunc_, quantities_) {}

    // Require the quantity (compute if needed)
    void require() { quantityD.require(); }

    // Unrequire the quantity
    void unrequire() { quantityD.unrequire(); }

    // Access the underlying data
    T& get() { return *quantityD.dataBuffer; }
    const T& get() const { return *quantityD.dataBuffer; }

  private:
    DependentQuantityD<T> quantityD;
  };

  // Register a custom managed quantity
  // Usage:
  //   auto myQuantity = geometry.registerCustomManagedQuantity<FaceData<double>>(
  //       myData, [&]() { /* compute myData */ });
  //   myQuantity->require();
  //   double value = myQuantity->get()[someFace];
  template <typename T>
  std::shared_ptr<CustomManagedQuantity<T>> registerCustomManagedQuantity(
      T& dataBuffer, std::function<void()> computeFunc);

  // Hide copy and move constructors; users are more likely to use them accidentally than intentionally.
  // See the explicit copy() function in derived classes.
  BaseGeometryInterface(const BaseGeometryInterface& other) = delete;
  BaseGeometryInterface& operator=(const BaseGeometryInterface& other) = delete;
  BaseGeometryInterface(BaseGeometryInterface&& other) = delete;
  BaseGeometryInterface& operator=(BaseGeometryInterface&& other) = delete;

  // === Quantities


  // == Indices
  // Note: These don't depend on any geometric information, and are no different than the getVertexIndices() offered by
  // the mesh class. However, its useful to offer them here so they can be used with the caching system.

  // Vertex indices
  VertexData<size_t> vertexIndices;
  void requireVertexIndices();
  void unrequireVertexIndices();

  // Interior vertex indices
  VertexData<size_t> interiorVertexIndices;
  void requireInteriorVertexIndices();
  void unrequireInteriorVertexIndices();

  // Edge indices
  EdgeData<size_t> edgeIndices;
  void requireEdgeIndices();
  void unrequireEdgeIndices();

  // Halfedge indices
  HalfedgeData<size_t> halfedgeIndices;
  void requireHalfedgeIndices();
  void unrequireHalfedgeIndices();

  // Corner indices
  CornerData<size_t> cornerIndices;
  void requireCornerIndices();
  void unrequireCornerIndices();

  // Face indices
  FaceData<size_t> faceIndices;
  void requireFaceIndices();
  void unrequireFaceIndices();

  // Boundary loop indices
  BoundaryLoopData<size_t> boundaryLoopIndices;
  void requireBoundaryLoopIndices();
  void unrequireBoundaryLoopIndices();


protected:
  // All of the quantities available (subclasses will also add quantities to this list)
  // Note that this is a vector of non-owning pointers; the quantities are generally value members in the class, so
  // there is no need to delete these.
  std::vector<DependentQuantity*> quantities;

  // Storage for custom managed quantities (owned pointers)
  // We need to keep these alive for the lifetime of the geometry object
  std::vector<std::shared_ptr<void>> customQuantityStorage;

  // === Implementation details for quantities

  // == Indices

  DependentQuantityD<VertexData<size_t>> vertexIndicesQ;
  virtual void computeVertexIndices();

  DependentQuantityD<VertexData<size_t>> interiorVertexIndicesQ;
  virtual void computeInteriorVertexIndices();

  DependentQuantityD<EdgeData<size_t>> edgeIndicesQ;
  virtual void computeEdgeIndices();

  DependentQuantityD<HalfedgeData<size_t>> halfedgeIndicesQ;
  virtual void computeHalfedgeIndices();

  DependentQuantityD<CornerData<size_t>> cornerIndicesQ;
  virtual void computeCornerIndices();

  DependentQuantityD<FaceData<size_t>> faceIndicesQ;
  virtual void computeFaceIndices();

  DependentQuantityD<BoundaryLoopData<size_t>> boundaryLoopIndicesQ;
  virtual void computeBoundaryLoopIndices();
};

// Template implementation for custom managed quantities
template <typename T>
std::shared_ptr<typename BaseGeometryInterface::CustomManagedQuantity<T>>
BaseGeometryInterface::registerCustomManagedQuantity(T& dataBuffer, std::function<void()> computeFunc) {
  auto customQuantity = std::make_shared<CustomManagedQuantity<T>>(&dataBuffer, computeFunc, quantities);
  customQuantityStorage.push_back(customQuantity);
  return customQuantity;
}

} // namespace surface
} // namespace geometrycentral
