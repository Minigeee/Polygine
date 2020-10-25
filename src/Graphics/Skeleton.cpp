#include <poly/Core/Logger.h>

#include <poly/Graphics/Skeleton.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace poly
{

namespace priv
{


Matrix4f aiMatrixToMatrix4f(const aiMatrix4x4& t)
{
#ifdef USE_COLUMN_MAJOR
	return Matrix4f(
		t.a1, t.b1, t.c1, t.d1,
		t.a2, t.b2, t.c2, t.d2,
		t.a3, t.b3, t.c3, t.d3,
		t.a4, t.b4, t.c4, t.d4
	);
#else
	return Matrix4f(
		t.a1, t.a2, t.a3, t.a4,
		t.b1, t.b2, t.b3, t.b4,
		t.c1, t.c2, t.c3, t.c4,
		t.d1, t.d2, t.d3, t.d4
	);
#endif
}


///////////////////////////////////////////////////////////
void getBoneOffsets(aiNode* node, const aiScene* scene, HashMap<std::string, Matrix4f>& offsets)
{
	// For each mesh in the node, add the list of bones
	for (Uint32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		for (Uint32 b = 0; b < mesh->mNumBones; ++b)
		{
			// Add bone name
			aiBone* bone = mesh->mBones[b];
			offsets[bone->mName.C_Str()] = aiMatrixToMatrix4f(bone->mOffsetMatrix);
		}
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		getBoneOffsets(node->mChildren[i], scene, offsets);
}


///////////////////////////////////////////////////////////
void addBones(aiNode* node, Bone* parent, const aiScene* scene, Skeleton* skeleton, const HashMap<std::string, Matrix4f>& offsets)
{
	std::string name = node->mName.C_Str();
	Bone* bone = 0;

	auto it = offsets.find(name);
	if (it != offsets.end())
	{
		// Create bone
		bone = skeleton->createBone(name);

		// Start with bind pose
		Matrix4f transform = aiMatrixToMatrix4f(node->mTransformation);
		bone->setTransform(transform);

		// Set offset matrix
		bone->setOffset(it.value());

		// Set root node
		if (!parent)
			skeleton->setRoot(bone, transform);
		else
			parent->addBone(bone);
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		addBones(node->mChildren[i], bone, scene, skeleton, offsets);
}


}


///////////////////////////////////////////////////////////
Skeleton::Skeleton() :
	m_root				(0),
	m_bonePool			(sizeof(Bone), 10),
	m_globalInverseBind	(1.0f)
{

}


///////////////////////////////////////////////////////////
bool Skeleton::load(const std::string& fname)
{
	// Load the scene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fname, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_WARNING("Failed to load skeleton file %s: %s", fname.c_str(), importer.GetErrorString());
		return false;
	}

	// Get bone offsets
	HashMap<std::string, Matrix4f> offsets;
	priv::getBoneOffsets(scene->mRootNode, scene, offsets);

	// Create skeleton
	priv::addBones(scene->mRootNode, 0, scene, this, offsets);

	return true;
}


///////////////////////////////////////////////////////////
Bone* Skeleton::createBone(const std::string& name)
{
	// Create bone
	Bone* bone = (Bone*)m_bonePool.alloc();
	*bone = Bone(name, m_boneMap.size());

	// Store bone
	m_boneMap[name] = bone;

	return bone;
}


///////////////////////////////////////////////////////////
void Skeleton::removeBone(const std::string& name)
{
	auto it = m_boneMap.find(name);

	// Remove if the bone exists
	if (it != m_boneMap.end())
	{
		Bone* bone = it.value();

		// Remove from parent bone
		if (bone->getParent())
			bone->getParent()->removeBone(name);

		// Remove from pool
		bone->~Bone();
		m_bonePool.free(bone);

		// Remove from map
		m_boneMap.erase(it);
	}
}


///////////////////////////////////////////////////////////
void Skeleton::setRoot(Bone* bone, const Matrix4f& transform)
{
	m_root = bone;
	m_globalInverseBind = inverse(transform);
}


///////////////////////////////////////////////////////////
Bone* Skeleton::getBone(const std::string& name) const
{
	Bone* bone = 0;

	auto it = m_boneMap.find(name);
	if (it != m_boneMap.end())
		bone = it.value();

	return bone;
}


///////////////////////////////////////////////////////////
Bone* Skeleton::getRoot() const
{
	return m_root;
}


///////////////////////////////////////////////////////////
Uint32 Skeleton::getNumBones() const
{
	return m_boneMap.size();
}


///////////////////////////////////////////////////////////
void Skeleton::apply(Shader* shader)
{
	for (auto it = m_boneMap.begin(); it != m_boneMap.end(); ++it)
	{
		Bone* bone = it.value();

		// Calculate final transform
		Matrix4f transform = bone->getGlobalTransform() * bone->getOffset();

		// Set bone transform
		shader->setUniform("u_bones[" + std::to_string(bone->getId()) + "]", transform);
	}
}


}