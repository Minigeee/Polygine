#include <poly/Graphics/Bone.h>

namespace poly
{


///////////////////////////////////////////////////////////
Bone::Bone() :
	m_id				(-1),
	m_name				("bone"),
	m_parent			(0),
	m_offset			(1.0f),
	m_localTransform	(1.0f),
	m_globalTransform	(1.0f),
	m_transformDirty	(false)
{

}


///////////////////////////////////////////////////////////
Bone::Bone(const std::string& name, int id) :
	m_id				(id),
	m_name				(name),
	m_parent			(0),
	m_offset			(1.0f),
	m_localTransform	(1.0f),
	m_globalTransform	(1.0f),
	m_transformDirty	(false)
{

}


///////////////////////////////////////////////////////////
void Bone::addBone(Bone* bone)
{
	// Don't add the bone if it already has a parent
	if (!bone->m_parent)
	{
		// Set parent
		bone->m_parent = this;

		// Add as child
		m_children.push_back(bone);

		// Mark transforms as dirty
		bone->markTransformsDirty();
	}
}


///////////////////////////////////////////////////////////
void Bone::removeBone(const std::string& name)
{
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if ((*it)->m_name == name)
			m_children.erase(it);
	}
}


///////////////////////////////////////////////////////////
void Bone::setOffset(const Matrix4f& offset)
{
	m_offset = offset;
}


///////////////////////////////////////////////////////////
void Bone::setTransform(const Matrix4f& transform)
{
	m_localTransform = transform;

	// Mark transforms as dirty
	markTransformsDirty();
}


///////////////////////////////////////////////////////////
int Bone::getId() const
{
	return m_id;
}


///////////////////////////////////////////////////////////
const std::string& Bone::getName() const
{
	return m_name;
}


///////////////////////////////////////////////////////////
Bone* Bone::getParent() const
{
	return m_parent;
}


///////////////////////////////////////////////////////////
const std::vector<Bone*>& Bone::getChildren() const
{
	return m_children;
}


///////////////////////////////////////////////////////////
const Matrix4f& Bone::getOffset() const
{
	return m_offset;
}


///////////////////////////////////////////////////////////
const Matrix4f& Bone::getLocalTransform() const
{
	return m_localTransform;
}


///////////////////////////////////////////////////////////
const Matrix4f& Bone::getGlobalTransform()
{
	if (m_transformDirty)
	{
		if (m_parent)
			m_globalTransform = m_parent->getGlobalTransform() * m_localTransform;
		else
			m_globalTransform = m_localTransform;

		m_transformDirty = false;
	}

	return m_globalTransform;
}


///////////////////////////////////////////////////////////
void Bone::markTransformsDirty()
{
	m_transformDirty = true;

	for (Uint32 i = 0; i < m_children.size(); ++i)
		m_children[i]->markTransformsDirty();
}


}