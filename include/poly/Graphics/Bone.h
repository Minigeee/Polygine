#ifndef POLY_BONE_H
#define POLY_BONE_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Matrix4.h>

#include <string>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A class that acts as a skeleton node and stores transform data
///
///////////////////////////////////////////////////////////
class Bone
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Bone();

	///////////////////////////////////////////////////////////
	/// \brief Create a bone from a name and an id
	///
	/// \param name The name of the bone
	/// \param id The id of the bone
	///
	///////////////////////////////////////////////////////////
	Bone(const std::string& name, int id);

	///////////////////////////////////////////////////////////
	/// \brief Add a bone as a child of the current bone
	///
	/// \param bone The child bone to add
	///
	///////////////////////////////////////////////////////////
	void addBone(Bone* bone);

	///////////////////////////////////////////////////////////
	/// \brief Remove a child bone
	///
	/// \param name Name of the bone to remove
	///
	///////////////////////////////////////////////////////////
	void removeBone(const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Get the bone offset matrix
	///
	/// The offset matrix transforms vertices from model coordinate
	/// space to the bone's local coordinate space.
	///
	/// \param offset The offset matrix
	///
	///////////////////////////////////////////////////////////
	void setOffset(const Matrix4f& offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the bone's local transform matrix
	///
	/// The local transform matrix transforms vertices from the
	/// bone's local coordinate space to its parent's local
	/// coordinate space.
	///
	/// \param local The local transform matrix
	///
	///////////////////////////////////////////////////////////
	void setTransform(const Matrix4f& local);

	///////////////////////////////////////////////////////////
	/// \brief Get the bone id
	///
	/// \return The bone id
	///
	///////////////////////////////////////////////////////////
	int getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the bone name
	///
	/// \return The bone name
	///
	///////////////////////////////////////////////////////////
	const std::string& getName() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the bone's parent bone
	///
	/// \return The parent bone
	///
	///////////////////////////////////////////////////////////
	Bone* getParent() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the list of children bones
	///
	/// \return A list of children bones
	///
	///////////////////////////////////////////////////////////
	const std::vector<Bone*>& getChildren() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the offset matrix
	///
	/// The offset matrix transforms vertices from model coordinate
	/// space to the bone's local coordinate space.
	///
	/// \return The offset matrix
	///
	///////////////////////////////////////////////////////////
	const Matrix4f& getOffset() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the local transform matrix
	///
	/// The local transform matrix transforms vertices from the
	/// bone's local coordinate space to its parent's local
	/// coordinate space.
	///
	/// \return The local transform matrix
	///
	///////////////////////////////////////////////////////////
	const Matrix4f& getLocalTransform() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the bone global transform matrix
	///
	/// The global tranform matrix transforms vertices from the
	/// bone's local coordinate space to the model's coordinate
	/// space.
	///
	/// \return The global tranform matrix
	///
	///////////////////////////////////////////////////////////
	const Matrix4f& getGlobalTransform();

private:
	void markTransformsDirty();

private:
	int m_id;						//!< The bone id
	std::string m_name;				//!< The bone name
	Bone* m_parent;					//!< The parent bone
	std::vector<Bone*> m_children;	//!< The list of children bones

	Matrix4f m_offset;				//!< The offset matrix
	Matrix4f m_localTransform;		//!< The local transform matrix
	Matrix4f m_globalTransform;		//!< The global transform matrix
	bool m_transformDirty;			//!< True if the local transform was changed
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Bone
/// \ingroup Graphics
///
/// A bone represents a single node in a skeleton tree, where
/// each bone has child bones and a parent bone, unless it is
/// the root node. Each bone has a name, an id, an offset matrix,
/// and a local and global transform matrix. The offset matrix
/// transforms vertices from model coordinate space to the bone's
/// local coordinate space. The local transform matrix transforms
/// vertices from the bone's local coordinate space to its parent's
/// space. The global transform matrix transforms vertices from
/// local space to model space.
///
/// In most cases, it will not be necessary to directly interact
/// with a bone, as it is easier to use a Skeleton instead.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Bone torso("Torso", 0)
/// Bone arm("Arm_R", 1);
///
/// torso.addBone(&arm);
///
/// torso.getChildren()[0];		// "Arm_R"
/// arm.getParent();			// "Torso"
/// torso.getParent();			// 0
///
/// \endcode
///
///////////////////////////////////////////////////////////