#ifndef POLY_SOUND_H
#define POLY_SOUND_H

#include <poly/Audio/AudioSource.h>

#include <poly/Core/Time.h>

#include <SFML/Audio/Sound.hpp>

namespace poly
{


class AudioBuffer;


///////////////////////////////////////////////////////////
/// \brief An audio source that can play audio stored in a buffer
///
///////////////////////////////////////////////////////////
class Sound : public AudioSource
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Sound();

	///////////////////////////////////////////////////////////
	/// \brief Start playback of the sound
	///
	/// This function starts playback of the sound starting from
	/// the beggining if the sound was stopped or already playing,
	/// and resumes from the previous position if the sound was paused.
	/// A valid buffer must be assigned to the sound with setBuffer()
	/// for this function to work.
	///
	///////////////////////////////////////////////////////////
	void play() override;

	///////////////////////////////////////////////////////////
	/// \brief Pause playback of the sound
	///
	/// Playback of the sound is paused, and the playback offset
	/// is kept in the same position.
	///
	///////////////////////////////////////////////////////////
	void pause();

	///////////////////////////////////////////////////////////
	/// \brief Stop playback of the sound
	///
	/// Playback of the sound is stopped and the playback offset
	/// is reset to the beggining.
	///
	///////////////////////////////////////////////////////////
	void stop() override;

	///////////////////////////////////////////////////////////
	/// \brief Set the sound audio buffer
	///
	/// This function sets the audio buffer to pull audio data from
	/// for playback.
	///
	/// \param buffer A pointer to the audio buffer
	///
	///////////////////////////////////////////////////////////
	void setBuffer(AudioBuffer* buffer);

	///////////////////////////////////////////////////////////
	/// \brief Set whether or not the sound should loop to the beggining once playback reaches the end
	///
	/// \param loop True to loop playback to the beggining at the end
	///
	///////////////////////////////////////////////////////////
	void setLoop(bool loop);

	///////////////////////////////////////////////////////////
	/// \brief Set the sound playback offset in units of time
	///
	/// \param offset The playback offset in units of time
	///
	///////////////////////////////////////////////////////////
	void setPlayingOffset(Time offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the pitch of the sound
	///
	/// \copydetails AudioSource::setPitch
	///
	///////////////////////////////////////////////////////////
	void setPitch(float pitch) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the volume of the sound
	///
	/// \copydetails AudioSource::setVolume
	///
	///////////////////////////////////////////////////////////
	void setVolume(float volume) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the sound for spatial audio
	///
	/// \copydetails AudioSource::setPosition
	///
	///////////////////////////////////////////////////////////
	void setPosition(const Vector3f& pos) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the sound for spatial audio
	///
	/// \copydetails AudioSource::setPosition(float,float,float)
	///
	///////////////////////////////////////////////////////////
	void setPosition(float x, float y, float z) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the minimum distance of the sound for spatial audio
	///
	/// \copydetails AudioSource::setMinDistance
	///
	///////////////////////////////////////////////////////////
	void setMinDistance(float dist) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the atteunuation factor of the sound for spatial audio
	///
	/// \copydetails AudioSource::setAttenuation
	///
	///////////////////////////////////////////////////////////
	void setAttenuation(float attenuation) override;

	///////////////////////////////////////////////////////////
	/// \brief Get the status of the sound
	///
	/// \copydetails AudioSource::getStatus
	///
	///////////////////////////////////////////////////////////
	AudioSource::Status getStatus() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the audio buffer the sound reads data from during playback
	///
	/// \return A pointer to the audio buffer
	///
	///////////////////////////////////////////////////////////
	AudioBuffer* getBuffer() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the sound is looped
	///
	/// \return True if the sound is looped
	///
	///////////////////////////////////////////////////////////
	bool getLoop() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the playback offset of the sound
	///
	/// \return The playback offset of the sound
	///
	///////////////////////////////////////////////////////////
	Time getPlayingOffset() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the pitch of the sound
	///
	/// \copydetails AudioSource::getPitch
	///
	///////////////////////////////////////////////////////////
	float getPitch() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the volume of the sound
	///
	/// \copydetails AudioSource::getVolume
	///
	///////////////////////////////////////////////////////////
	float getVolume() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the position of the sound when using spatial audio
	///
	/// \copydetails AudioSource::getPosition
	///
	///////////////////////////////////////////////////////////
	Vector3f getPosition() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the minimum distance of the sound when using spatial audio
	///
	/// \copydetails AudioSource::getMinDistance
	///
	///////////////////////////////////////////////////////////
	float getMinDistance() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the attenuation factor of the sound when using spatial audio
	///
	/// \copydetails AudioSource::getAttenuation
	///
	///////////////////////////////////////////////////////////
	float getAttenuation() const override;

private:
	sf::Sound m_sound;			//!< The internal
	AudioBuffer* m_buffer;		//!< A pointer to the audio buffer
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Sound
/// \ingroup Audio
///
/// A sound should be used to play short sounds in memory easily with
/// little latency. Uncompressed audio data takes up quite a bit of
/// memory so storing long audio files in memory is not viable. This
/// is why sounds used in this class should be relatively short.
/// Music should be used for longer audio files.
///
/// To use a sound, the audio data must be set up in an AudioBuffer
/// first, and the audio buffer must be connected to the sound using
/// setBuffer(). The buffer object must remain in the same location in
/// memory while the sound is being played.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load audio data
/// AudioBuffer buffer;
/// buffer.open("audio/footsteps.wav");
///
/// // Play a sound
/// Sound sound;
/// sound.setBuffer(&buffer);
/// sound.play();
///
/// // Wait some time...
///
/// sound.pause();
///
/// \endcode
///
///////////////////////////////////////////////////////////