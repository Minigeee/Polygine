#include <poly/Core/Logger.h>

#include <poly/Graphics/Animation.h>
#include <poly/Graphics/Skeleton.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace poly
{

namespace priv
{


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
			skeleton->setRoot(bone);
		else
			parent->addBone(bone);
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		addBones(node->mChildren[i], bone, scene, skeleton, offsets);
}


///////////////////////////////////////////////////////////
void applyAnimation(Bone* bone, Animation* animation, float time)
{
	// Apply transform
	bone->setTransform(animation->getTransform(bone->getName(), time));
	int test = 0;

	// Apply transform for children
	for (Uint32 i = 0; i < bone->getChildren().size(); ++i)
		applyAnimation(bone->getChildren()[i], animation, time);
}


///////////////////////////////////////////////////////////
void freeBone(Bone* bone, ObjectPool& pool)
{
	// Skip if the bone does not exist
	if (!bone) return;

	// Free children first
	for (Uint32 i = 0; i < bone->getChildren().size(); ++i)
		freeBone(bone->getChildren()[i], pool);

	// Call bone destructor
	bone->~Bone();

	// Free from the object pool
	pool.free(bone);
}


///////////////////////////////////////////////////////////
Bone* copyBone(Skeleton* skeleton, Bone* bone, Bone* parent, ObjectPool& pool)
{
	// Skip if the source bone does not exist
	if (!bone) return 0;

	// Create a new bone
	Bone* newBone = skeleton->createBone(bone->getName());

	// Set offset and local transform
	newBone->setOffset(bone->getOffset());
	newBone->setTransform(bone->getLocalTransform());

	// Add to parent bone
	if (parent)
		parent->addBone(newBone);

	// For each child bone from the original bone, copy the child bone
	for (Uint32 i = 0; i < bone->getChildren().size(); ++i)
		copyBone(skeleton, bone->getChildren()[i], newBone, pool);

	// Return the new bone
	return newBone;
}


}


///////////////////////////////////////////////////////////
UniformBuffer Skeleton::s_uniformBuffer;


///////////////////////////////////////////////////////////
UniformBuffer& Skeleton::getUniformBuffer()
{
	if (!s_uniformBuffer.getId())
		// Create a buffer with 2 MB space
		s_uniformBuffer.create(2 * 1024 * 1024);

	return s_uniformBuffer;
}


///////////////////////////////////////////////////////////
Skeleton::Skeleton() :
	m_root				(0),
	m_bonePool			(sizeof(Bone), 10),
	m_animation			(0),
	m_animTime			(0.0f),
	m_animSpeed			(1.0f),
	m_uniformOffset		(0)
{

}


///////////////////////////////////////////////////////////
Skeleton::Skeleton(const std::string& fname) :
	m_root				(0),
	m_bonePool			(sizeof(Bone), 10),
	m_animation			(0),
	m_animTime			(0.0f),
	m_animSpeed			(1.0f),
	m_uniformOffset		(0)
{
	load(fname);
}


///////////////////////////////////////////////////////////
Skeleton::Skeleton(const Skeleton& skeleton) :
	m_root				(0),
	m_bonePool			(sizeof(Bone), 10),
	m_animation			(skeleton.m_animation),
	m_animTime			(skeleton.m_animTime),
	m_animSpeed			(skeleton.m_animSpeed),
	m_uniformOffset		(0)
{
	// Do a depth first search and copy all bones to the new object pool
	m_root = priv::copyBone(this, skeleton.m_root, 0, m_bonePool);
}


///////////////////////////////////////////////////////////
Skeleton& Skeleton::operator=(const Skeleton& skeleton)
{
	if (this != &skeleton)
	{
		// Free all previous bones
		priv::freeBone(m_root, m_bonePool);

		// Reset the object pool
		m_bonePool.reset();
		m_animation = skeleton.m_animation;
		m_animTime = skeleton.m_animTime;
		m_animSpeed = skeleton.m_animSpeed;
		m_uniformOffset = 0;

		// Do a depth first search and copy all bones to the new object pool
		m_root = priv::copyBone(this, skeleton.m_root, 0, m_bonePool);
	}

	return *this;
}


///////////////////////////////////////////////////////////
Skeleton::~Skeleton()
{
	// Call all bone destructors
	priv::freeBone(m_root, m_bonePool);
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
void Skeleton::apply(Shader* shader)
{
	// Bind uniform buffer
	shader->bindUniformBlock("Skeleton", getUniformBuffer(), m_uniformOffset, sizeof(UniformBlock_Skeleton));
}


///////////////////////////////////////////////////////////
void Skeleton::update(float dt)
{
	// Only apply atnimation if root node exists and animation exists
	if (m_root && m_animation)
	{
		// Update animation time
		float duration = m_animation->getDuration() / m_animation->getTicksPerSecond();
		m_animTime += fmodf(dt * m_animSpeed, duration);
		if (m_animTime < 0.0f)
			m_animTime += duration;

		// Recursively apply animation
		priv::applyAnimation(m_root, m_animation, m_animTime);

		// Add bones to the uniform buffer
		UniformBlock_Skeleton block;
		for (auto it = m_boneMap.begin(); it != m_boneMap.end(); ++it)
		{
			Bone* bone = it.value();

			// Calculate final transform
			Matrix4f transform = bone->getGlobalTransform() * bone->getOffset();

			// Set bone transform
			block.m_bones[bone->getId()] = transform;
		}

		// Keep track of uniform offset
		m_uniformOffset = getUniformBuffer().pushData(block);
	}
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
void Skeleton::setRoot(Bone* bone)
{
	m_root = bone;
}


///////////////////////////////////////////////////////////
void Skeleton::setAnimation(Animation* animation)
{
	m_animation = animation;
	m_animTime = 0.0f;
}


///////////////////////////////////////////////////////////
void Skeleton::setAnimationTime(float time)
{
	m_animTime = time;
}


///////////////////////////////////////////////////////////
void Skeleton::setAnimationSpeed(float speed)
{
	m_animSpeed = speed;
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
Animation* Skeleton::getAnimation() const
{
	return m_animation;
}


///////////////////////////////////////////////////////////
float Skeleton::getAnimationTime() const
{
	return m_animTime;
}


///////////////////////////////////////////////////////////
float Skeleton::getAnimationSpeed() const
{
	return m_animSpeed;
}


}