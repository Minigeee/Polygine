#ifndef POLY_SKELETON_H
#define POLY_SKELETON_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Bone.h>
#include <poly/Graphics/Shader.h>

namespace poly
{

class Bone;

class Skeleton
{
public:
	Skeleton();

	bool load(const std::string& fname);

	Bone* createBone(const std::string& name);

	void removeBone(const std::string& name);

	void setRoot(Bone* bone, const Matrix4f& transform);

	Bone* getBone(const std::string& name) const;

	Bone* getRoot() const;

	Uint32 getNumBones() const;

	void apply(Shader* shader);
	
private:
	Bone* m_root;
	ObjectPool m_bonePool;
	HashMap<std::string, Bone*> m_boneMap;

public:
	Matrix4f m_globalInverseBind;
};

}

#endif