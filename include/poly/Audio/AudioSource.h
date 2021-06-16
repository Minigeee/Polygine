#ifndef POLY_AUDIO_SOURCE_H
#define POLY_AUDIO_SOURCE_H

#include <poly/Math/Vector3.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief The base abstract class for audio sources
///
///////////////////////////////////////////////////////////
class AudioSource
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The status of the audio source
	///
	///////////////////////////////////////////////////////////
	enum Status
	{
		Stopped,		//!< The audio source is stopped
		Paused,			//!< The audio source is paused
		Playing			//!< The audio source is playing
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~AudioSource() { }

	///////////////////////////////////////////////////////////
	/// \brief Start or resume playback of the audio source
	///
	/// The audio source starts playback if it was stopped, and
	/// restarts if it was already playing (for sources that can
	/// change playback position).
	///
	///////////////////////////////////////////////////////////
	virtual void play() = 0;

	///////////////////////////////////////////////////////////
	/// \brief Stop audio playback
	///
	/// This functions stops audio playback and resets the playback
	/// position to the beggining (for sources that can change
	/// playback position).
	///
	///////////////////////////////////////////////////////////
	virtual void stop() = 0;

	///////////////////////////////////////////////////////////
	/// \brief Set the pitch of the audio source
	///
	/// The pitch represents the percieved frequency of the audio,
	/// but this will change the speed of the sound as well.
	///
	/// The default value is 1.0.
	///
	/// \param pitch The new pitch to apply to the audio source
	///
	///////////////////////////////////////////////////////////
	virtual void setPitch(float pitch) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Set the volume of the audio source
	///
	/// The volume is the loudness or amplitude of the audio. The
	/// volume should be a decimal value between 0 (no sound) to
	/// 100 (max sound).
	///
	/// The default value is 100.0.
	///
	/// \param volume The new volume to apply to the audio source
	///
	///////////////////////////////////////////////////////////
	virtual void setVolume(float volume) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the audio source for spatial audio
	///
	/// Only sounds that have a single channel can be used in spatial audio.
	/// The default audio position is (0, 0, 0).
	///
	/// \param pos The 3D position of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual void setPosition(const Vector3f& pos) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the audio source for spatial audio
	///
	/// Only sounds that have a single channel can be used in spatial audio.
	/// The default audio position is (0, 0, 0).
	///
	/// \param x The x-component of the audio position
	/// \param y The y-component of the audio position
	/// \param z The z-component of the audio position
	///
	///////////////////////////////////////////////////////////
	virtual void setPosition(float x, float y, float z) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Set the minimum distance of the audio source for spatial audio
	///
	/// The minimum distance is the minimum distance away from the
	/// audio source a listener will hear an audio source at full volume.
	/// Past this distance, the volume will fade according to the attenuation
	/// factor. A minimum distance of negative or zero is not allowed.
	///
	/// The default distance is 1.0.
	///
	/// \param dist The new minimum distance of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual void setMinDistance(float dist) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Set the atteunuation factor of the audio source for spatial audio
	///
	/// The attenuation factor determines how quickly the volume of
	/// an audio source fades out as it gets further from the listener.
	/// A higher attenuation factor leads to a faster fade out, and
	/// an attenuation factor of 0 means that volume is the same no
	/// matter the distance from the listener.
	///
	/// The exact equation is:
	/// \code
	/// volumeFactor = minDist / (minDist + attenuation * (max(distance, minDist) - minDist))
	/// \endcode
	///
	/// The default value is 1.0.
	///
	/// \param attenuation The new attenuation factor of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual void setAttenuation(float attenuation) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Get the status of the audio source
	///
	/// \return The Status of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual Status getStatus() const = 0;

	///////////////////////////////////////////////////////////
	/// \brief Get the pitch of the audio source
	///
	/// \return The pitch of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual float getPitch() const = 0;

	///////////////////////////////////////////////////////////
	/// \brief Get the volume of the audio source
	///
	/// \return The volume of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual float getVolume() const = 0;

	///////////////////////////////////////////////////////////
	/// \brief Get the position of the audio source when using spatial audio
	///
	/// \return The position of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual Vector3f getPosition() const = 0;

	///////////////////////////////////////////////////////////
	/// \brief Get the minimum distance of the audio source when using spatial audio
	///
	/// \return The minimum distance of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual float getMinDistance() const = 0;

	///////////////////////////////////////////////////////////
	/// \brief Get the attenuation factor of the audio source when using spatial audio
	///
	/// \return The attenuation factor of the audio source
	///
	///////////////////////////////////////////////////////////
	virtual float getAttenuation() const = 0;
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::AudioSource
/// \ingroup Audio
///
/// \see Sound
/// \see Music
/// \see AudioStream
///
///////////////////////////////////////////////////////////