#ifndef POLY_ANIMATION_H
#define POLY_ANIMATION_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Matrix4.h>
#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

#include <string>
#include <vector>

namespace poly
{

#ifndef DOXYGEN_SKIP
#ifdef USE_COLUMN_MAJOR
#define ASSIMP_TO_POLY_MAT4(t) Matrix4f( \
	t.a1, t.b1, t.c1, t.d1, \
	t.a2, t.b2, t.c2, t.d2, \
	t.a3, t.b3, t.c3, t.d3, \
	t.a4, t.b4, t.c4, t.d4 \
);
#else
#define ASSIMP_TO_POLY_MAT4(t) Matrix4f( \
	t.a1, t.a2, t.a3, t.a4, \
	t.b1, t.b2, t.b3, t.b4, \
	t.c1, t.c2, t.c3, t.c4, \
	t.d1, t.d2, t.d3, t.d4 \
);
#endif
#endif


///////////////////////////////////////////////////////////
/// \brief A class that contains a sequence of keyframes that animates a skeleton
///
///////////////////////////////////////////////////////////
class Animation
{
public:
	///////////////////////////////////////////////////////////
	/// \brief A channel is a sequence of keyframes representing the animation sequence for a single bone
	///
	///////////////////////////////////////////////////////////
	struct Channel
	{
		std::vector<float> m_times;				//!< A list of keyframe times
		std::vector<Vector3f> m_positions;		//!< A list of bone positions
		std::vector<Quaternion> m_rotations;	//!< A list of bone rotations
		std::vector<Vector3f> m_scales;			//!< A list of bone scales
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Animation();

	///////////////////////////////////////////////////////////
	/// \brief Load an animation from a model file
	///
	/// \param fname The model file to load the animation from
	/// \param fname The name of the animation to load
	///
	/// \see load()
	///
	///////////////////////////////////////////////////////////
	Animation(const std::string& fname, const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Load an animation from a model file
	///
	/// Loading an animation from a file only works for model file
	/// formats that are supported by ASSIMP and can hold animation
	/// data (i.e. COLLADA/DAE). Only one animation can be loaded
	/// from a file per animation object.
	///
	/// \param fname The file name of the animation file
	/// \param name The name of the animation to load
	///
	/// \return True if the animation was loaded without errors
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname, const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Add a sequence of keyframes for a single bone
	///
	/// Each keyframe in the sequence must have a time, position.
	/// rotation, and scale, meaning that the size of all the
	/// sequences must be the same.
	///
	/// \param bone The name of the bone to add the channel for
	/// \param channel The channel with keyframe data
	///
	///////////////////////////////////////////////////////////
	void addChannel(const std::string& bone, const Channel& channel);

	///////////////////////////////////////////////////////////
	/// \brief Remove a sequence of keyframes for a single bone
	///
	/// \param bone The name of the bone to remove the channel from
	///
	///////////////////////////////////////////////////////////
	void removeChannel(const std::string& bone);

	///////////////////////////////////////////////////////////
	/// \brief Set animation name
	///
	/// Tha animation name is just for convenience purposes
	///
	/// \param name The name
	///
	///////////////////////////////////////////////////////////
	void setName(const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Set the animation duration in number of ticks
	///
	/// \param duration The number of ticks the animation should last
	///
	///////////////////////////////////////////////////////////
	void setDuration(float duration);

	///////////////////////////////////////////////////////////
	/// \brief Set the number of ticks per seconds
	///
	/// \param tps The ticks per second
	///
	///////////////////////////////////////////////////////////
	void setTicksPerSecond(float tps);

	///////////////////////////////////////////////////////////
	/// \brief Get the animation name
	///
	/// \return The animation name string
	///
	///////////////////////////////////////////////////////////
	const std::string& getName() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the bone local transform matrix at the specified time
	///
	/// This matrix is the bone's local transform matrix, so it
	/// transforms vertices from the bone's local coordinate space
	/// to its parent's coordinate space. The matrix is calculated
	/// by interpolating between keyframes based on which time was
	/// requested in the animation sequence.
	///
	/// \param bone The name of the bone to retrieve a transform matrix for
	/// \param time The time into the animation sequence to retrieve data for (in seconds)
	///
	/// \return The bone's local transform matrix
	///
	///////////////////////////////////////////////////////////
	Matrix4f getTransform(const std::string& bone, float time) const;

	///////////////////////////////////////////////////////////
	/// \brief Get animation duration
	///
	/// The duration can be retrieved in either seconds or ticks.
	///
	/// \param inSeconds Set this to true to retrieve the duration in seconds, otherwise it is returned in ticks
	///
	/// \return The animation duration
	///
	///////////////////////////////////////////////////////////
	float getDuration(bool inSeconds = false) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of ticks per second
	///
	/// \return Number of ticks per second
	///
	///////////////////////////////////////////////////////////
	float getTicksPerSecond() const;

private:
	std::string m_name;							//!< The name of the animation
	float m_duration;							//!< The duration of the animation in ticks
	float m_ticksPerSecond;						//!< Number of ticks per seconds

	HashMap<std::string, Channel> m_channels;	//!< A map of bone names to channels
	HashMap<std::string, Matrix4f> m_bindPose;	//!< Some bones will miss channels if they are not animated, so use bind pose for that
};

}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Animation
/// \ingroup Graphics
///
/// An animation is a set of keyframe sequences for each bone
/// in a skeleton, where a keyframe defines the position, rotation,
/// and scale of a single bone at some time in the sequence.
/// Each bone has its own channel that defines local transforms
/// that can be queried to create skeletal animation.
///
/// To load an animation, use the load() function to load an
/// animation from a model file. This function internally uses
/// the ASSIMP model loading library, so for an animation to be
/// loaded from a file, the file format must support the storage
/// of animation data, and it must be a file format that is supported
/// by ASSIMP. The most common file format that meets these
/// requirements is COLLADA/DAE.
///
/// To apply the animation to a skeleton, use Skeleton::setAnimation(),
/// and call Skeleton::update() every frame. Internally, the
/// skeleton will call getTransform() every frame for every bone
/// contained within the skeleton, and this will update the
/// local and global transforms of the bone every frame. This
/// bone transform data can then be pushed to a shader to animate
/// a model.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load the animation
/// Animation walk;
/// walk.load("model.dae", "Walk");
///
/// // Load a skeleton
/// Skeleton skeleton;
/// skeleton.load("model.dae");
///
/// // Apply tha animation
/// skeleton.setAnimation(&animation);
///
/// // To play the animation in reverse, uncomment below
/// // skeleton.setAnimationSpeed(-1.0f);
///
/// // Play the animation
/// Clock clock;
/// while (true)
///		skeleton.update(clock.restart().toSeconds());
///
/// \endcode
///
///////////////////////////////////////////////////////////