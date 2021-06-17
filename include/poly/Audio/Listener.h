#ifndef POLY_LISTENER_H
#define POLY_LISTENER_H

#include <poly/Math/Vector3.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief The audio listener that is used to represent the location and orientation where all sounds are heard from
///
///////////////////////////////////////////////////////////
class Listener
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Set the global volume that applies to audio sources
	///
	/// The global volume is combined with the individual volumes of
	/// each audio source. This value must be a decimal between 0
	/// and 100.
	///
	/// The default value is 100.0.
	///
	/// \param The new global volume
	///
	///////////////////////////////////////////////////////////
	static void setGlobalVolume(float volume);

	///////////////////////////////////////////////////////////
	/// \brief Set the position of the listener where all audio sources are heard from
	///
	/// The default position is (0, 0, 0).
	///
	/// \param pos The new listener position
	///
	///////////////////////////////////////////////////////////
	static void setPosition(const Vector3f& pos);

	///////////////////////////////////////////////////////////
	/// \brief Set the position of the listener where all audio sources are heard from
	///
	/// The default position is (0, 0, 0).
	///
	/// \param x The x-coordinate of the listener position
	/// \param y The y-coordinate of the listener position
	/// \param z The z-coordinate of the listener position
	///
	///////////////////////////////////////////////////////////
	static void setPosition(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Set the direction the listener is facing
	///
	/// The direction is the forward vector of the listner and
	/// determines where the listener is facing. This vector, along
	/// with the up vector, define the orientation of the listener and
	/// determines directional sound. The vector does not need to
	/// be normalized.
	///
	/// \param dir The new front vector of the listener
	///
	///////////////////////////////////////////////////////////
	static void setDirection(const Vector3f& dir);

	///////////////////////////////////////////////////////////
	/// \brief Set the direction the listener is facing
	///
	/// The direction is the forward vector of the listner and
	/// determines where the listener is facing. This vector, along
	/// with the up vector, define the orientation of the listener and
	/// determines directional sound. The vector does not need to
	/// be normalized.
	///
	/// \param x The x-component of the front vector
	/// \param y The y-component of the front vector
	/// \param z The z-component of the front vector
	///
	///////////////////////////////////////////////////////////
	static void setDirection(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Set the up vector of the listener
	///
	/// The up vector points from the listener's location towards
	/// the positive y-axis in the listener's local coordniate system.
	/// This vector, along with the front vector, define the orientation
	/// of the listener and determines directional sound. The vector
	/// does not need to be normalized.
	///
	/// \param dir The new up vector of the listener
	///
	///////////////////////////////////////////////////////////
	static void setUpVector(const Vector3f& up);

	///////////////////////////////////////////////////////////
	/// \brief Set the up vector of the listener
	///
	/// The up vector points from the listener's location towards
	/// the positive y-axis in the listener's local coordniate system.
	/// This vector, along with the front vector, define the orientation
	/// of the listener and determines directional sound. The vector
	/// does not need to be normalized.
	///
	/// \param x The x-component of the up vector
	/// \param y The y-component of the up vector
	/// \param z The z-component of the up vector
	///
	///////////////////////////////////////////////////////////
	static void setUpVector(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Get the global volume
	///
	/// \return The global volume
	///
	///////////////////////////////////////////////////////////
	static float getGlobalVolume();

	///////////////////////////////////////////////////////////
	/// \brief Get the position of the listener for spatial audio
	///
	/// \return The position of the listener
	///
	///////////////////////////////////////////////////////////
	static Vector3f getPosition();

	///////////////////////////////////////////////////////////
	/// \brief Get the front vector of the listener for spatial audio
	///
	/// \return The front vector of the listener
	///
	///////////////////////////////////////////////////////////
	static Vector3f getDirection();

	///////////////////////////////////////////////////////////
	/// \brief Get the up vector of the listener for spatial audio
	///
	/// \return The up vector of the listener
	///
	///////////////////////////////////////////////////////////
	static Vector3f getUpVector();
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Listener
/// \ingroup Audio
///
/// The audio listener controls the properties of the main listener
/// in a 3D audio scene, and is used for spatial audio. The position,
/// direction, and global volume can be controlled using the listener.
/// All functions are static because there is only one audio output.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Set position
/// Listener::setPosition(0.0f, 10.0f, 5.0f);
///
/// // Set direction
/// Listener::setDirection(0.0f, -1.0f, 2.0f);
///
/// \endcode
///
///////////////////////////////////////////////////////////