#include <poly/Core/Logger.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Image.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/Window.h>

#include <poly/Graphics/Shaders/animated.vert.h>
#include <poly/Graphics/Shaders/default.vert.h>
#include <poly/Graphics/Shaders/default.frag.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace poly
{


///////////////////////////////////////////////////////////
Shader Model::s_defaultShader;

///////////////////////////////////////////////////////////
Shader Model::s_animatedShader;

///////////////////////////////////////////////////////////
ObjectPool texturePool(sizeof(Texture), 50);

///////////////////////////////////////////////////////////
HashMap<std::string, Texture*> s_textureMap;


///////////////////////////////////////////////////////////
ModelLoadSettings::ModelLoadSettings() :
	m_scale				(1.0f),
	m_adjustForGamma	(1.0f),
	m_flatShading		(true)
{

}


///////////////////////////////////////////////////////////
Mesh::Mesh() :
	m_shader	(0)
{

}


///////////////////////////////////////////////////////////
Shader& Model::getDefaultShader()
{
	if (!s_defaultShader.getId())
	{
		s_defaultShader.load("poly/default.vert", SHADER_DEFAULT_VERT, Shader::Vertex);
		s_defaultShader.load("poly/default.frag", SHADER_DEFAULT_FRAG, Shader::Fragment);
		s_defaultShader.compile();
	}

	return s_defaultShader;
}


///////////////////////////////////////////////////////////
Shader& Model::getAnimatedShader()
{
	if (!s_animatedShader.getId())
	{
		s_animatedShader.load("poly/animated.vert", SHADER_ANIMATED_VERT, Shader::Vertex);
		s_animatedShader.load("poly/default.frag", SHADER_DEFAULT_FRAG, Shader::Fragment);
		s_animatedShader.compile();
	}

	return s_animatedShader;
}


namespace priv
{

#ifndef DOXYGEN_SKIP


///////////////////////////////////////////////////////////
SkeletalData::SkeletalData() :
	m_boneWeights	(0.0f),
	m_boneIds		(-1)
{

}


///////////////////////////////////////////////////////////
struct ModelLoadState
{
	std::vector<Vertex> m_vertices;
	std::vector<SkeletalData> m_skeletalData;
	std::vector<Uint32> m_indices;
	std::vector<Mesh*>* m_meshes;
	std::vector<Uint32> m_vertexOffsets;
	HashMap<std::string, int> m_bones;

	const aiScene* m_scene;
	std::string m_directory;
	const ModelLoadSettings* m_settings;
};

#endif


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
	auto it = s_textureMap.find(path);
	if (it != s_textureMap.end())
		return it.value();

	// Create the texture
	Texture*& texture = s_textureMap[path];
	texture = (Texture*)texturePool.alloc();

	// Load texture
	if (!texture->load(path, GLType::Uint8, TextureFilter::Linear, TextureWrap::Repeat, true, state.m_settings->m_adjustForGamma))
	{
		s_textureMap.erase(path);
		return 0;
	}

	return texture;
}


///////////////////////////////////////////////////////////
void processMaterial(aiMaterial* material, ModelLoadState& state)
{
	Material& modelMat = state.m_meshes->back()->m_material;

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
	modelMat.setDiffTexture(loadMaterialTexture(material, aiTextureType_DIFFUSE, state));
	modelMat.setSpecTexture(loadMaterialTexture(material, aiTextureType_SPECULAR, state));
}


///////////////////////////////////////////////////////////
void processMesh(aiMesh* mesh, ModelLoadState& state)
{
	// If there are bones, resize the skeletal data array
	if (mesh->mNumBones)
		state.m_skeletalData.resize(mesh->mNumVertices);

	// Keep track of mesh vertex offsets
	Uint32 vertexOffset = state.m_settings->m_flatShading ? state.m_vertices.size() : state.m_indices.size();
	state.m_vertexOffsets.push_back(vertexOffset);

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

	// Scale vertices
	const Vector3f& scale = state.m_settings->m_scale;

	// Process vertices
	for (Uint32 i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector3D& p = mesh->mVertices[i];
		aiVector3D& n = mesh->mNormals[i];
		aiVector3D& t = mesh->mTangents[i];

		// Position, normal, tangent
		Vertex vertex;
		vertex.m_position.x = p.x * scale.x;
		vertex.m_position.y = p.y * scale.y;
		vertex.m_position.z = p.z * scale.z;
		vertex.m_normal.x = n.x;
		vertex.m_normal.y = n.y;
		vertex.m_normal.z = n.z;
		vertex.m_tangent.x = t.x;
		vertex.m_tangent.y = t.y;
		vertex.m_tangent.z = t.z;

		// Texture coords
		if (mesh->mTextureCoords[0])
		{
			aiVector3D& uv = mesh->mTextureCoords[0][i];
			vertex.m_texCoord.x = uv.x;
			vertex.m_texCoord.y = uv.y;
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
		state.m_vertices.push_back(vertex);
	}

	// Load indices array if flat shading is disabled
	if (!state.m_settings->m_flatShading)
	{
		for (Uint32 i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace& face = mesh->mFaces[i];
			for (Uint32 j = 0; j < face.mNumIndices; ++j)
				state.m_indices.push_back(face.mIndices[j]);
		}
	}

	// Create the mesh
	state.m_meshes->push_back(Pool<Mesh>::alloc());

	// Add material
	if (state.m_settings->m_loadMaterials && mesh->mMaterialIndex >= 0)
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
		it.value() = numBones++;

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
Model::Model()
{

}


///////////////////////////////////////////////////////////
Model::Model(const std::string& fname, const ModelLoadSettings& settings)
{
	load(fname, settings);
}


///////////////////////////////////////////////////////////
Model::~Model()
{
	for (Uint32 i = 0; i < m_meshes.size(); ++i)
		Pool<Mesh>::free(m_meshes[i]);
}


///////////////////////////////////////////////////////////
bool Model::load(const std::string& fname, const ModelLoadSettings& settings)
{
	// Load the model scene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fname,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

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
	state.m_meshes = &m_meshes;
	state.m_settings = &settings;

	// Process bone data
	int numBones = 0;
	priv::getBoneNames(scene->mRootNode, scene, state.m_bones);
	priv::getBoneIds(scene->mRootNode, scene, state.m_bones, numBones);

	// Process nodes
	priv::processNode(scene->mRootNode, state);


	// Create vertex buffer
	m_vertices = std::move(state.m_vertices);
	m_indices = std::move(state.m_indices);

	// Create bounding box
	m_boundingBox = priv::calcBoundingBox(m_vertices);

	// Create bounding sphere
	m_boundingSphere.m_position = m_boundingBox.getCenter();
	m_boundingSphere.m_radius = length(m_boundingBox.getDimensions()) * 0.5f;

	// Move the state data into temporary lists
	m_skeletalData = std::move(state.m_skeletalData);
	m_meshVertexOffsets = std::move(state.m_vertexOffsets);

	// Finish loading model
	finish();

	LOG("Loaded model: %s", fname.c_str());
	return true;
}


///////////////////////////////////////////////////////////
bool Model::finish()
{
	// Don't finish if vertex buffer is already created
	if (m_vertexBuffer.getId())
		return true;

	if (!Window::isContextActive())
		return false;

	// Get a default shader
	Shader* shader = m_skeletalData.size() ? &getAnimatedShader() : &getDefaultShader();

	m_vertexBuffer.create(m_vertices);

	// Setup indices array if smooth shading enabled
	if (m_indices.size())
		m_indicesBuffer.create(m_indices);

	// Create skeletal data buffer
	// NOTE : Skeletal data for only a single mesh is supported atm
	if (m_skeletalData.size())
	{
		// Create vertex buffer
		m_skeletalVertexBuffer.create(m_skeletalData);

		// Add attributes
		for (Uint32 i = 0; i < m_meshes.size(); ++i)
		{
			VertexArray& vao = m_meshes[i]->m_vertexArray;
			vao.addBuffer(m_skeletalVertexBuffer, 9, 4, sizeof(priv::SkeletalData), 0 * sizeof(float));
			vao.addBuffer(m_skeletalVertexBuffer, 10, 4, sizeof(priv::SkeletalData), 4 * sizeof(float), 0, GLType::Int32);
		}
	}

	// Create vertex arrays
	for (Uint32 i = 0; i < m_meshes.size(); ++i)
	{
		// Calculate buffer offset
		Uint32 offset = m_meshVertexOffsets[i];
		Uint32 size = (i == m_meshes.size() - 1 ? m_vertices.size() - offset : m_meshVertexOffsets[i + 1] - offset);
		offset *= sizeof(Vertex);

		VertexArray& vao = m_meshes[i]->m_vertexArray;
		vao.addBuffer(m_vertexBuffer, 0, 3, sizeof(Vertex), offset + 0 * sizeof(float));
		vao.addBuffer(m_vertexBuffer, 1, 3, sizeof(Vertex), offset + 3 * sizeof(float));
		vao.addBuffer(m_vertexBuffer, 2, 2, sizeof(Vertex), offset + 6 * sizeof(float));
		vao.addBuffer(m_vertexBuffer, 3, 4, sizeof(Vertex), offset + 8 * sizeof(float));
		vao.addBuffer(m_vertexBuffer, 4, 3, sizeof(Vertex), offset + 12 * sizeof(float));

		if (m_indices.size())
			vao.setElementBuffer(m_indicesBuffer);

		// Set vertex info
		vao.setNumVertices(size);
		vao.setVertexOffset(0);

		// Set the default shader if it has no shader
		if (!m_meshes[i]->m_shader)
			m_meshes[i]->m_shader = shader;

		// Finish loading textures
		Material& material = m_meshes[i]->m_material;
		if (material.getDiffTexture())
			material.getDiffTexture()->finish();
		if (material.getSpecTexture())
			material.getSpecTexture()->finish();
		if (material.getNormalTexture())
			material.getNormalTexture()->finish();
	}

	// Unbind indices buffer to reset state
	if (m_indices.size())
		m_indicesBuffer.unbind();

	// Clear the temporary data
	m_skeletalData.clear();
	m_meshVertexOffsets.clear();

	return true;
}


///////////////////////////////////////////////////////////
void Model::addMesh(const std::vector<Vertex>& vertices, const std::vector<Uint32>& indices, const Material& material, BufferUsage usage)
{
	// Keep track of the vertex offsets
	Uint32 vertexOffset = m_vertices.size();
	Uint32 indexOffset = m_indices.size();
	Uint32 offset = indices.size() ? m_indices.size() : m_vertices.size();
	Uint32 size = indices.size() ? indices.size() : vertices.size();

	// Append this list of vertices to the end of the other
	m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	m_indices.insert(m_indices.end(), indices.begin(), indices.end());
	m_vertexBuffer.create(m_vertices, usage);

	// If indices are being used, update the index buffer
	if (indices.size())
	{
		// Adjust the index values, since there may be data previously in the buffer
		for (Uint32 i = indexOffset; i < m_indices.size(); ++i)
			m_indices[i] += vertexOffset;

		m_indicesBuffer.bind(BufferTarget::Element);
		m_indicesBuffer.create(m_indices, usage);
	}

	// Create a single mesh
	m_meshes.push_back(Pool<Mesh>::alloc());
	Mesh* mesh = m_meshes.back();
	VertexArray& vao = mesh->m_vertexArray;

	// Create vertex array
	vao.addBuffer(m_vertexBuffer, 0, 3, sizeof(Vertex), 0 * sizeof(float));
	vao.addBuffer(m_vertexBuffer, 1, 3, sizeof(Vertex), 3 * sizeof(float));
	vao.addBuffer(m_vertexBuffer, 2, 2, sizeof(Vertex), 6 * sizeof(float));
	vao.addBuffer(m_vertexBuffer, 3, 4, sizeof(Vertex), 8 * sizeof(float));
	vao.addBuffer(m_vertexBuffer, 4, 3, sizeof(Vertex), 12 * sizeof(float));
	vao.addBuffer(m_vertexBuffer, 5, 3, sizeof(Vertex), 15 * sizeof(float));

	if (indices.size())
		vao.setElementBuffer(m_indicesBuffer);

	// Set vertex info
	vao.setNumVertices(size);
	vao.setVertexOffset(offset);

	// Set the material
	mesh->m_material = material;

	// Set a default shader
	mesh->m_shader = &getDefaultShader();

	// Create bounding box
	BoundingBox box = priv::calcBoundingBox(vertices);

	if (box.m_min.x < m_boundingBox.m_min.x)
		m_boundingBox.m_min.x = box.m_min.x;
	if (box.m_max.x > m_boundingBox.m_max.x)
		m_boundingBox.m_max.x = box.m_max.x;

	if (box.m_min.y < m_boundingBox.m_min.y)
		m_boundingBox.m_min.y = box.m_min.y;
	if (box.m_max.y > m_boundingBox.m_max.y)
		m_boundingBox.m_max.y = box.m_max.y;

	if (box.m_min.z < m_boundingBox.m_min.z)
		m_boundingBox.m_min.z = box.m_min.z;
	if (box.m_max.z > m_boundingBox.m_max.z)
		m_boundingBox.m_max.z = box.m_max.z;

	// Create bounding sphere
	m_boundingSphere.m_position = m_boundingBox.getCenter();
	m_boundingSphere.m_radius = length(m_boundingBox.getDimensions()) * 0.5f;
}


///////////////////////////////////////////////////////////
void Model::setMaterial(const Material& material, Uint32 index)
{
	m_meshes[index]->m_material = material;
}


///////////////////////////////////////////////////////////
void Model::setShader(Shader* shader, Uint32 index)
{
	m_meshes[index]->m_shader = shader;
}


///////////////////////////////////////////////////////////
const std::vector<Vertex>& Model::getVertices() const
{
	return m_vertices;
}


///////////////////////////////////////////////////////////
const std::vector<Uint32>& Model::getIndices() const
{
	return m_indices;
}


///////////////////////////////////////////////////////////
Uint32 Model::getNumMeshes() const
{
	return m_meshes.size();
}


///////////////////////////////////////////////////////////
Mesh* Model::getMesh(Uint32 index) const
{
	return m_meshes[index];
}


}