#ifndef POLY_CONVEX_MESH_COLLIDER_H
#define POLY_CONVEX_MESH_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


class Model;
struct Vertex;


class ConvexMeshCollider : public Collider
{
public:
	ConvexMeshCollider();

	ConvexMeshCollider(const Model& model, Uint32 mesh = 0);

	void setModel(const Model& model, Uint32 mesh = 0);

	void setVertices(const std::vector<Vertex>& vertices, const std::vector<Uint32>& indices = std::vector<Uint32>());

	Uint32 getNumVertices() const;

	Uint32 getNumUniqueVertices() const;

	Vertex* getVertices() const;

	Uint32* getIndices() const;

private:
	Uint32 m_numVertices;
	Uint32 m_numUniqueVertices;
	Vertex* m_vertices;
	Uint32* m_indices;
};


}

#endif