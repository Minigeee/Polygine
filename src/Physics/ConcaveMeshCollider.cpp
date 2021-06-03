#include <poly/Graphics/Model.h>

#include <poly/Physics/ConcaveMeshCollider.h>

namespace poly
{


///////////////////////////////////////////////////////////
ConcaveMeshCollider::ConcaveMeshCollider() :
	m_vertices			(0),
	m_indices			(0),
	m_numVertices		(0),
	m_numUniqueVertices	(0)
{

}


///////////////////////////////////////////////////////////
ConcaveMeshCollider::ConcaveMeshCollider(const Model& model, Uint32 mesh) :
	m_vertices			(0),
	m_indices			(0),
	m_numVertices		(0),
	m_numUniqueVertices	(0)
{
	setModel(model, mesh);
}


///////////////////////////////////////////////////////////
void ConcaveMeshCollider::setModel(const Model& model, Uint32 mesh)
{
	if (mesh >= model.getNumMeshes()) return;

	Uint32 numVertices = model.getIndices().size() ? model.getIndices().size() : model.getVertices().size();
	m_numVertices = numVertices - model.getMesh(mesh)->m_offset;
	if (mesh + 1 < model.getNumMeshes())
		m_numVertices -= numVertices - model.getMesh(mesh + 1)->m_offset;

	// Number of unique vertices
	m_numUniqueVertices = model.getIndices().size() ? model.getVertices().size() : m_numVertices;

	// The vertex offset will be different based on if indices are enabled or not
	Uint32 vertexOffset = model.getIndices().size() ? 0 : model.getMesh()->m_offset;

	m_vertices = model.getVertices().size() ? const_cast<Vertex*>(&model.getVertices()[vertexOffset]) : 0;
	m_indices = model.getIndices().size() ? const_cast<Uint32*>(&model.getIndices()[model.getMesh()->m_offset]) : 0;
}


///////////////////////////////////////////////////////////
void ConcaveMeshCollider::setVertices(const std::vector<Vertex>& vertices, const std::vector<Uint32>& indices)
{
	m_numVertices = indices.size() ? indices.size() : vertices.size();
	m_vertices = m_numVertices ? const_cast<Vertex*>(&vertices[0]) : 0;
	m_indices = m_numVertices ? const_cast<Uint32*>(&indices[0]) : 0;
}


///////////////////////////////////////////////////////////
Uint32 ConcaveMeshCollider::getNumVertices() const
{
	return m_numVertices;
}


///////////////////////////////////////////////////////////
Uint32 ConcaveMeshCollider::getNumUniqueVertices() const
{
	return m_numUniqueVertices;
}


///////////////////////////////////////////////////////////
Vertex* ConcaveMeshCollider::getVertices() const
{
	return m_vertices;
}


///////////////////////////////////////////////////////////
Uint32* ConcaveMeshCollider::getIndices() const
{
	return m_indices;
}


}