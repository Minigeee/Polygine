#ifndef POLY_BONE_H
#define POLY_BONE_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Matrix4.h>

#include <string>

namespace poly
{

class Bone
{
public:
	Bone();
	Bone(const std::string& name, int id);

	void addBone(Bone* bone);

	void removeBone(const std::string& name);

	void setOffset(const Matrix4f& offset);

	void setTransform(const Matrix4f& local);

	int getId() const;

	const std::string& getName() const;

	Bone* getParent() const;

	const std::vector<Bone*>& getChildren() const;

	const Matrix4f& getOffset() const;

	const Matrix4f& getLocalTransform() const;

	const Matrix4f& getGlobalTransform();

private:
	void markTransformsDirty();

private:
	int m_id;
	std::string m_name;
	Bone* m_parent;
	std::vector<Bone*> m_children;

	Matrix4f m_offset;
	Matrix4f m_localTransform;
	Matrix4f m_globalTransform;
	bool m_transformDirty;
};

}

#endif