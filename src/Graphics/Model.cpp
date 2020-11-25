#include <poly/Core/Logger.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Image.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Texture.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace poly
{


///////////////////////////////////////////////////////////
ObjectPool texturePool(sizeof(Texture), 50);

///////////////////////////////////////////////////////////
HashMap<std::string, Texture*> textureMap;


namespace priv
{


///////////////////////////////////////////////////////////
struct SkeletalData
{
	SkeletalData() :
		m_boneWeights	(0.0f),
		m_boneIds		(-1)
	{ }

	Vector4f m_boneWeights;
	Vector4i m_boneIds;
};


///////////////////////////////////////////////////////////
struct ModelLoadState
{
	std::vector<Vertex> m_vertices;
	std::vector<SkeletalData> m_skeletalData;
	std::vector<Material> m_materials;
	HashMap<std::string, int> m_bones;

	const aiScene* m_scene;
	std::string m_directory;
};


///////////////////////////////////////////////////////////
Texture* loadMaterialTexture(aiMaterial* material, aiTextureType type, ModelLoadState& state)
{
	// Only 1 texture per type is supported currently
	if (!material->GetTextureCount(type)) return 0;

	// Get texture path
	aiString fname;
	material->GetTexture(type, 0, &fname);
	std::string path = state.m_directory  + '/' + fname.C_Str();

	// If the texture has already been loaded, return it
	auto it = textureMap.find(path);
	if (it != textureMap.end())
		return it.value();

	// Otherwise load the image
	Image image;
	if (!image.load(path))
		return 0;

	// Create the texture
	Texture*& texture = textureMap[path];
	texture = (Texture*)texturePool.alloc();
	texture->create(image);

	return texture;
}


///////////////////////////////////////////////////////////
void processMaterial(aiMaterial* material, ModelLoadState& state)
{
	Material modelMat;

	// Diffuse
	aiColor3D diffuse;
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	modelMat.setDiffuse(diffuse.r, diffuse.g, diffuse.b);

	// Specular
	aiColor3D specular;
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	modelMat.setSpecular(specular.r, specular.g, specular.b);

	// Specular strength
	float specStregth = 1.0f;
	material->Get(AI_MATKEY_SHININESS_STRENGTH, specStregth);
	modelMat.getSpecular() *= specStregth;

	// Shininess
	float specFactor;
	material->Get(AI_MATKEY_SHININESS, specFactor);
	modelMat.setShininess(specFactor);

	// Load textures
	Uint32 numMaterials = state.m_materials.size();
	modelMat.addTexture(
		"u_diffuseMaps[" + std::to_string(numMaterials) + ']',
		loadMaterialTexture(material, aiTextureType_DIFFUSE, state)
	);
	modelMat.addTexture(
		"u_specularMaps" + std::to_string(numMaterials) + ']',
		loadMaterialTexture(material, aiTextureType_SPECULAR, state)
	);

	// Add material
	state.m_materials.push_back(modelMat);
}


///////////////////////////////////////////////////////////
void processMesh(aiMesh* mesh, ModelLoadState& state)
{
	// If there are bones, resize the skeletal data array
	if (mesh->mNumBones)
		state.m_skeletalData.resize(mesh->mNumVertices);

	// Process bones
	for (Uint32 i = 0; i < mesh->mNumBones; ++i)
	{
		aiBone* bone = mesh->mBones[i];
		int id = state.m_bones[bone->mName.C_Str()];

		// Process bone weights and ids
		for (Uint32 w = 0; w < bone->mNumWeights; ++w)
		{
			aiVertexWeight& weight = bone->mWeights[w];

			SkeletalData& data = state.m_skeletalData[weight.mVertexId];
			float* boneWeight = (float*)&data.m_boneWeights;
			int* boneId = (int*)&data.m_boneIds;

			// Find the min weight and id
			float* minWeight = boneWeight;
			int* minId = boneId;

			for (int j = 1; j < 4; ++j)
			{
				if (boneWeight[j] < *minWeight)
				{
					minWeight = boneWeight + j;
					minId = boneId + j;
				}
			}

			// If the weight is less than the vertex min weight, replace it
			if (weight.mWeight > *minWeight)
			{
				*minWeight = weight.mWeight;
				*minId = i;
			}
		}
	}

	// Normalize all skeletal data
	for (Uint32 i = 0; i < state.m_skeletalData.size(); ++i)
	{
		Vector4f& weight = state.m_skeletalData[i].m_boneWeights;
		weight /= sum(weight);
	}

	// Process vertices
	for (Uint32 i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector3D& p = mesh->mVertices[i];
		aiVector3D& n = mesh->mNormals[i];

		// Position and normal
		Vertex vertex;
		vertex.m_position.x = p.x;
		vertex.m_position.y = p.y;
		vertex.m_position.z = p.z;
		vertex.m_normal.x = n.x;
		vertex.m_normal.y = n.y;
		vertex.m_normal.z = n.z;

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

		// Material index
		vertex.m_material = state.m_materials.size();

		// Add to list
		state.m_vertices.push_back(vertex);
	}

	// Add material
	if (mesh->mMaterialIndex >= 0)
		processMaterial(state.m_scene->mMaterials[mesh->mMaterialIndex], state);
}


///////////////////////////////////////////////////////////
void processNode(aiNode* node, ModelLoadState& state)
{
	// Process all meshes in the node
	for (Uint32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = state.m_scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, state);
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], state);
}


///////////////////////////////////////////////////////////
void getBoneNames(aiNode* node, const aiScene* scene, HashMap<std::string, int>& bones)
{
	// For each mesh in the node, add the list of bones
	for (Uint32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		for (Uint32 b = 0; b < mesh->mNumBones; ++b)
		{
			// Add bone name
			aiBone* bone = mesh->mBones[b];
			bones[bone->mName.C_Str()] = 0;
		}
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		getBoneNames(node->mChildren[i], scene, bones);
}


///////////////////////////////////////////////////////////
void getBoneIds(aiNode* node, const aiScene* scene, HashMap<std::string, int>& bones, int& numBones)
{
	// If the node name matches a bone name, set the id
	auto it = bones.find(node->mName.C_Str());
	if (it != bones.end())
	{
		it.value() = numBones++;
		LOG("%s: %d", node->mName.C_Str(), it.value());
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		getBoneIds(node->mChildren[i], scene, bones, numBones);
}


///////////////////////////////////////////////////////////
BoundingBox calcBoundingBox(const std::vector<Vertex>& vertices)
{
	// Return default box if there are no vertices
	if (!vertices.size())
		return BoundingBox();

	BoundingBox box(vertices[0].m_position, vertices[0].m_position);

	for (Uint32 i = 1; i < vertices.size(); ++i)
	{
		const Vector3f& p = vertices[i].m_position;

		if (p.x < box.m_min.x)
			box.m_min.x = p.x;
		else if (p.x > box.m_max.x)
			box.m_max.x = p.x;

		if (p.y < box.m_min.y)
			box.m_min.y = p.y;
		else if (p.y > box.m_max.y)
			box.m_max.y = p.y;

		if (p.z < box.m_min.z)
			box.m_min.z = p.z;
		else if (p.z > box.m_max.z)
			box.m_max.z = p.z;
	}

	return box;
}


}


///////////////////////////////////////////////////////////
Vertex::Vertex() :
	m_position	(0.0f),
	m_normal	(0.0f, 1.0f, 0.0f),
	m_texCoord	(0.0f),
	m_color		(1.0f),
	m_material	(0)
{ }


///////////////////////////////////////////////////////////
Vertex::Vertex(const Vector3f& pos, const Vector3f& normal) :
	m_position	(pos),
	m_normal	(normal),
	m_texCoord	(0.0f),
	m_color		(1.0f),
	m_material	(0)
{ }


///////////////////////////////////////////////////////////
Vertex::Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texCoord) :
	m_position	(pos),
	m_normal	(normal),
	m_texCoord	(texCoord),
	m_color		(1.0f),
	m_material	(0)
{ }


///////////////////////////////////////////////////////////
Vertex::Vertex(const Vector3f& pos, const Vector3f& normal, const Colorf& color) :
	m_position	(pos),
	m_normal	(normal),
	m_texCoord	(0.0f),
	m_color		(color),
	m_material	(0)
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

	// Initialize load state
	priv::ModelLoadState state;
	state.m_scene = scene;
	state.m_directory = fname.substr(0, fname.find_last_of("/\\"));

	// Process bone data
	int numBones = 0;
	priv::getBoneNames(scene->mRootNode, scene, state.m_bones);
	priv::getBoneIds(scene->mRootNode, scene, state.m_bones, numBones);

	// Process nodes
	priv::processNode(scene->mRootNode, state);


	// Create vertex buffer
	m_vertices = std::move(state.m_vertices);
	m_vertexBuffer.create(m_vertices);

	// Create vertex array
	m_vertexArray.addBuffer(m_vertexBuffer, 0, 3, sizeof(Vertex), 0 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 1, 3, sizeof(Vertex), 3 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 2, 2, sizeof(Vertex), 6 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 3, 4, sizeof(Vertex), 8 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 4, 1, sizeof(Vertex), 12 * sizeof(float), 0, GLType::Int32);

	// Create skeletal data buffer
	if (state.m_skeletalData.size())
	{
		// Create vertex buffer
		m_skeletalVertexBuffer.create(state.m_skeletalData);

		// Add attributes
		m_vertexArray.addBuffer(m_skeletalVertexBuffer, 5, 4, sizeof(priv::SkeletalData), 0 * sizeof(float));
		m_vertexArray.addBuffer(m_skeletalVertexBuffer, 6, 4, sizeof(priv::SkeletalData), 4 * sizeof(float), 0, GLType::Int32);
	}

	// Create bounding box
	m_boundingBox = priv::calcBoundingBox(m_vertices);

	// Set materials
	m_materials = state.m_materials;


	LOG("Loaded model: %s", fname.c_str());
	return true;
}


///////////////////////////////////////////////////////////
void Model::create(const std::vector<Vertex>& vertices, BufferUsage usage)
{
	m_vertices = vertices;
	m_vertexBuffer.create(vertices, usage);

	// Create vertex array
	m_vertexArray.addBuffer(m_vertexBuffer, 0, 3, sizeof(Vertex), 0 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 1, 3, sizeof(Vertex), 3 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 2, 2, sizeof(Vertex), 6 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 3, 4, sizeof(Vertex), 8 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 4, 1, sizeof(Vertex), 12 * sizeof(float), 0, GLType::Int32);

	// Create bounding box
	m_boundingBox = priv::calcBoundingBox(vertices);
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

	// Create bounding box
	m_boundingBox = priv::calcBoundingBox(vertices);
}


///////////////////////////////////////////////////////////
void Model::setMaterial(const Material& material, Uint32 index)
{
	m_materials[index] = material;
}


///////////////////////////////////////////////////////////
VertexArray& Model::getVertexArray()
{
	return m_vertexArray;
}


///////////////////////////////////////////////////////////
const BoundingBox& Model::getBoundingBox() const
{
	return m_boundingBox;
}


///////////////////////////////////////////////////////////
const std::vector<Vertex>& Model::getVertices() const
{
	return m_vertices;
}


///////////////////////////////////////////////////////////
const Material& Model::getMaterial(Uint32 index) const
{
	return m_materials[index];
}


///////////////////////////////////////////////////////////
const std::vector<Material>& Model::getMaterials() const
{
	return m_materials;
}


}