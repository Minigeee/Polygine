#ifndef POLY_SKELETON_H
#define POLY_SKELETON_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Bone.h>
#include <poly/Graphics/Shader.h>

namespace poly
{

class Animation;
class Bone;

///////////////////////////////////////////////////////////
/// \brief A class that contains bone transform data to animate models
///
///////////////////////////////////////////////////////////
class Skeleton
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Skeleton();

	///////////////////////////////////////////////////////////
	/// \brief Load a skeleton from a model file
	///
	/// \param fname The model file to load the skeleton from
	///
	/// \see load()
	///
	///////////////////////////////////////////////////////////
	Skeleton(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Correctly deinitializes bones from the object pool
	///
	///////////////////////////////////////////////////////////
	~Skeleton();

#ifndef DOXYGEN_SKIP
	Skeleton(const Skeleton& skeleton);
	Skeleton& operator=(const Skeleton& skeleton);
#endif

	///////////////////////////////////////////////////////////
	/// \brief Load a skeleton from a file
	///
	/// Any model file format that is able to be loaded by Assimp
	/// and has the capability of storing an armature skeleton
	/// will work with this function. This function will load the
	/// entire model file, but only the armature skeleton will be kept.
	/// An example of a common file format that would work with this
	/// function is Collada/DAE.
	///
	/// Skeletons that are loaded from a file will start in bind pose.
	///
	/// \param fname The file name
	///
	/// \return True if the skeleton file was loaded successfully
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Apply bone transform data to a shader
	///
	/// This function will apply the correct bone transforms to a shader
	/// so that the models that are being rendered will have the correct
	/// skeleton pose applied. Because only one skeleton can be
	/// applied to each shader at a time, each animated model has
	/// to be rendered separately. This means that animated models
	/// do not support instanced rendering, unless the skeleton
	/// is not applied to the shader. It is possible to render animated
	/// models without a skeleton, they will be rendered in their bind pose.
	///
	/// \param shader A pointer to the shader to apply transforms to
	///
	///////////////////////////////////////////////////////////
	void apply(Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Update the current animation that is applied to the skeleton
	///
	/// When the skeleton has an existing animation applied to it,
	/// this function will update all the skeleton's bone transforms
	/// based on the current animation time and the given elapsed time
	/// in seconds. This will not make any modifications to the
	/// animation, it will just calculate the bone transforms
	/// using the data provided by the animation object.
	///
	/// \param dt The elapsed frame time in seconds
	///
	///////////////////////////////////////////////////////////
	void update(float dt);

	///////////////////////////////////////////////////////////
	/// \brief Allocate and create a new bone using a pool allocator
	///
	/// This function uses a pool allocator to create a new bone,
	/// so that the memory location of each bone will not change after
	/// its initial creation.
	///
	/// \param name Name to assign the new bone
	///
	/// \return A pointer to the new bone
	///
	///////////////////////////////////////////////////////////
	Bone* createBone(const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Remove the bone from the bone map and free its memory from the object pool
	///
	/// This function removes the bone from the bone map, and it removes
	/// the object from the object pool. This will cause any pointers
	/// referencing the bone to be invalidated, but the bone will not
	/// be removed from any bone heirarchies it is in. If the specified
	/// bone does not exist, nothing happens.
	///
	/// \param name The name of the bone to remove
	///
	///////////////////////////////////////////////////////////
	void removeBone(const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Set the skeleton root bone
	///
	/// \param bone A pointer to the root bone
	///
	///////////////////////////////////////////////////////////
	void setRoot(Bone* bone);

	///////////////////////////////////////////////////////////
	/// \brief Apply an animation to the skeleton
	///
	/// Sets the skeleton animation pointer and resets the animation time
	/// time to zero seconds. Animations are always looped.
	///
	/// \param animation A pointer to the skeleton animation
	///
	///////////////////////////////////////////////////////////
	void setAnimation(Animation* animation);

	///////////////////////////////////////////////////////////
	/// \brief Set the animation time
	///
	/// The animation time is the amount of time elapsed since
	/// the beginning of the animation in seconds.
	///
	/// \param time The animation time seconds
	///
	///////////////////////////////////////////////////////////
	void setAnimationTime(float time);

	///////////////////////////////////////////////////////////
	/// \brief Set the animation speed
	///
	/// The animation is a time multiplier. Reversed animations can
	/// be achieved by using a negative speed.
	///
	/// \param speed The animation speed
	///
	///////////////////////////////////////////////////////////
	void setAnimationSpeed(float speed);

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to a bone created by this skeleton
	///
	/// \param name The name of the bone to retrieve
	///
	/// \return A pointer to the bone
	///
	///////////////////////////////////////////////////////////
	Bone* getBone(const std::string& name) const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to the root bone
	///
	/// \return A pointer to the root bone
	///
	///////////////////////////////////////////////////////////
	Bone* getRoot() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of bones created in this skeleton
	///
	/// This function does not count the number of bones attached to the root
	/// node, it counts the number of bones created and stored in
	/// the bone map.
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumBones() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the current animation
	///
	/// \return A pointer to the current animation
	///
	///////////////////////////////////////////////////////////
	Animation* getAnimation() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the current time into the animation loop
	///
	/// \return The elapsed animation time in seconds
	///
	///////////////////////////////////////////////////////////
	float getAnimationTime() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the animation speed
	///
	/// \return The animation speed multiplier
	///
	///////////////////////////////////////////////////////////
	float getAnimationSpeed() const;
	
private:
	Bone* m_root;							//!< The root node
	ObjectPool m_bonePool;					//!< The bone object pool
	HashMap<std::string, Bone*> m_boneMap;	//!< Maps bone name to bone objects

	Animation* m_animation;					//!< The current animation applied to the skeleton
	float m_animTime;						//!< The current time in the animation
	float m_animSpeed;						//!< The animation speed, or time multiplier
};

}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Skeleton
/// \ingroup Graphics
///
/// The skeleton class allocates and manages bone hierarchies
/// for skeletal animation, where each bone contains transform
/// data that can be used to animate models. The skeleton internally
/// uses an object pool to manage memory for creating bones, and
/// every time a new bone is created, its name is mapped to its
/// location.
///
/// To use a skeleton, use the load() function to load a skeleton
/// from a model file (i.e. COLLADA/DAE files). Any models that
/// can be loaded with ASSIMP will work with the skeleton load()
/// function. Some of these supported formats are:
///
/// \li DAE/Collada
/// \li STL
/// \li OBJ
/// \li FBX
/// \li 3DS
///
/// When the skeleton is loaded, all the bones will start in
/// their bind pose position. To access bone transform, use
/// getBone() to get a pointer to the bone object, but the bone
/// name must be known to access the bone.
///
/// To animate the skeleton, load an Animation and apply the 
/// animation to the skeleton using setSkeleton(), and call the
/// update() function every frame to apply transforms from the
/// animation to the skeleton bones.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load the skeleton
/// Skeleton skeleton;
/// skeleton.load("model.dae");
///
/// // Get a bone
/// Bone* rhand = skeleton.getBone("Hand_R");
///
/// // The bone global transform can be used for making equipment follow the bone
/// // (i.e. having a character equip a sword or shield)
/// const Matrix4f& transform = rhand.getGlobalTransform();
///
/// \endcode
///
///////////////////////////////////////////////////////////