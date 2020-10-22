#include <poly/Core/Logger.h>

#include <poly/Graphics/Model.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
void processMesh(aiMesh* mesh, std::vector<Vertex>& vertices, const aiScene* scene)
{
	// Process vertices
	for (Uint32 i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector3D& p = mesh->mVertices[i];
		aiVector3D& n = mesh->mNormals[i];

		// Position and normal
		Vertex vertex;
		vertex.m_position.x = p.x;
		vertex.m_position.y = p.z;
		vertex.m_position.z = -p.y;
		vertex.m_normal.x = n.x;
		vertex.m_normal.y = n.z;
		vertex.m_normal.z = -n.y;

		// Texture coords
		if (mesh->mTextureCoords[0])
		{
			aiVector3D& t = mesh->mTextureCoords[0][i];
			vertex.m_texCoord.x = t.x;
			vertex.m_texCoord.y = t.y;
		}
		else
		{
			vertex.m_texCoord.x = 0.0f;
			vertex.m_texCoord.y = 0.0f;
		}

		if (mesh->mColors[0])
		{
			aiColor4D& c = mesh->mColors[0][i];
			vertex.m_color.r = c.r;
			vertex.m_color.g = c.g;
			vertex.m_color.b = c.b;
			vertex.m_color.a = c.a;
		}
		else
		{
			vertex.m_color.r = 1.0f;
			vertex.m_color.g = 1.0f;
			vertex.m_color.b = 1.0f;
			vertex.m_color.a = 1.0f;
		}

		// Add to list
		vertices.push_back(vertex);
	}
}


///////////////////////////////////////////////////////////
void processNode(aiNode* node, std::vector<Vertex>& vertices, const aiScene* scene)
{
	// Process all meshes in the node
	for (Uint32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, vertices, scene);
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], vertices, scene);
}


}


///////////////////////////////////////////////////////////
Vertex::Vertex() :
	m_position	(0.0f),
	m_normal	(0.0f, 1.0f, 0.0f),
	m_texCoord	(0.0f),
	m_color		(1.0f)
{ }


///////////////////////////////////////////////////////////
Vertex::Vertex(const Vector3f& pos, const Vector3f& normal) :
	m_position	(pos),
	m_normal	(normal),
	m_texCoord	(0.0f),
	m_color		(1.0f)
{ }


///////////////////////////////////////////////////////////
Vertex::Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texCoord) :
	m_position	(pos),
	m_normal	(normal),
	m_texCoord	(texCoord),
	m_color		(1.0f)
{ }


///////////////////////////////////////////////////////////
Vertex::Vertex(const Vector3f& pos, const Vector3f& normal, const Colorf& color) :
	m_position	(pos),
	m_normal	(normal),
	m_texCoord	(0.0f),
	m_color		(color)
{ }


///////////////////////////////////////////////////////////
Vertex::Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texCoord, const Colorf& color) :
	m_position	(pos),
	m_normal	(normal),
	m_texCoord	(texCoord),
	m_color		(color)
{ }


///////////////////////////////////////////////////////////
bool Model::load(const std::string& fname)
{
	// Load the model scene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fname, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_WARNING("Failed to load model file %s: %s", fname.c_str(), importer.GetErrorString());
		return false;
	}


	std::vector<Vertex> vertices;

	// Process nodes
	priv::processNode(scene->mRootNode, vertices, scene);

	// Create model based on nodes
	create(vertices);

	LOG("Loaded model: %s", fname.c_str());
	return true;
}


///////////////////////////////////////////////////////////
void Model::create(const std::vector<Vertex>& vertices, BufferUsage usage)
{
	// Initialize only if vertex buffer has not been created yet
	if (!m_vertexBuffer.getId())
	{
		m_vertices = vertices;
		m_vertexBuffer.create(vertices, usage);

		// Create vertex array
		m_vertexArray.addBuffer(m_vertexBuffer, 0, 3, sizeof(Vertex), 0 * sizeof(float));
		m_vertexArray.addBuffer(m_vertexBuffer, 1, 3, sizeof(Vertex), 3 * sizeof(float));
		m_vertexArray.addBuffer(m_vertexBuffer, 2, 2, sizeof(Vertex), 6 * sizeof(float));
		m_vertexArray.addBuffer(m_vertexBuffer, 3, 4, sizeof(Vertex), 8 * sizeof(float));
	}
}


///////////////////////////////////////////////////////////
void Model::setVertices(const std::vector<Vertex>& vertices)
{
	// Don't set vertices if model hasn't been created yet
	if (!m_vertexBuffer.getId()) return;

	// Check if the number of new vertices is not equal to the previous number of vertices
	if (m_vertices.size() != vertices.size())
		// Create a new buffer if new data has more vertices
		m_vertexBuffer.create(vertices, m_vertexBuffer.getUsage());

	else
		m_vertexBuffer.update(vertices);

	// Update vector
	m_vertices = vertices;
}


///////////////////////////////////////////////////////////
VertexArray& Model::getVertexArray()
{
	return m_vertexArray;
}


///////////////////////////////////////////////////////////
const std::vector<Vertex>& Model::getVertices() const
{
	return m_vertices;
}


}