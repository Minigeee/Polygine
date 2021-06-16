#ifndef POLY_MUSIC_H
#define POLY_MUSIC_H

#include <poly/Audio/AudioSource.h>

#include <poly/Core/Stream.h>
#include <poly/Core/Time.h>

#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/Audio/SoundStream.hpp>

namespace poly
{

class Music;

#ifndef DOXYGEN_SKIP
namespace priv
{


///////////////////////////////////////////////////////////
class SfmlMusic : public sf::SoundStream
{
public:
	SfmlMusic(Music* music);

	~SfmlMusic();

	void init(Uint32 numChannels, Uint32 sampleRate);

private:
	bool onGetData(Chunk& data) override;

	void onSeek(sf::Time timeOffset);

private:
	Music* m_music;
	Int16* m_buffer;
};


}
#endif


///////////////////////////////////////////////////////////
/// \brief An audio source that plays audio data streamed directly from the file
///
///////////////////////////////////////////////////////////
class Music :
	public AudioSource,
	public ReadStream
{
	friend priv::SfmlMusic;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Music();

	///////////////////////////////////////////////////////////
	/// \brief The destructor
	///
	///////////////////////////////////////////////////////////
	~Music();

	///////////////////////////////////////////////////////////
	/// \brief Open an audio file for playback
	///
	/// This audio file, unlike audio files for a Sound, can be
	/// any length, as they are not stored in memory but instead
	/// retrieved and decoded as needed directly from the file.
	/// The acceptable file formats are: WAV, OGG/Vorbis, and FLAC.
	///
	/// \param fname The path of the file to open
	///
	/// \return True if the file was successfully opened
	///
	///////////////////////////////////////////////////////////
	bool open(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Start playback of the music
	///
	/// This function starts playback of the music starting from
	/// the beggining if the music was stopped or already playing,
	/// and resumes from the previous position if the music was paused.
	/// An audio file must be opened with open() before playing.
	///
	/// This function will play the audio in real time as well as
	/// push the data to any output streams that this stream has been
	/// piped to.
	///
	///////////////////////////////////////////////////////////
	void play() override;

	///////////////////////////////////////////////////////////
	/// \brief Pause playback of the music
	///
	/// Playback of the music is paused, and the playback offset
	/// is kept in the same position.
	///
	///////////////////////////////////////////////////////////
	void pause();

	///////////////////////////////////////////////////////////
	/// \brief Stop playback of the music
	///
	/// Playback of the music is stopped and the playback offset
	/// is reset to the beggining.
	///
	///////////////////////////////////////////////////////////
	void stop() override;

	///////////////////////////////////////////////////////////
	/// \brief Read audio samples from an opened music file
	///
	/// This function reads audio samples from the object if a valid
	/// audio file has been opened. This will read the specified amount
	/// of data into the output buffer and advance the playback offset
	/// by the same amount. Data in this function is read in bytes,
	/// which is half the size of a sample.
	///
	/// \param buffer The output buffer to read the data into
	/// \param max The maximum number of bytes to read
	///
	/// \return The actual number of bytes that was read
	///
	///////////////////////////////////////////////////////////
	Uint32 read(void* buffer, Uint32 max) override;

	///////////////////////////////////////////////////////////
	/// \brief Set whether or not the music should loop to the beggining once playback reaches the end
	///
	/// \param loop True to loop playback to the beggining at the end
	///
	///////////////////////////////////////////////////////////
	void setLoop(bool loop);

	///////////////////////////////////////////////////////////
	/// \brief Set the music playback offset in units of time
	///
	/// \param offset The playback offset in units of time
	///
	///////////////////////////////////////////////////////////
	void setPlayingOffset(Time offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the pitch of the music
	///
	/// \copydetails AudioSource::setPitch
	///
	///////////////////////////////////////////////////////////
	void setPitch(float pitch) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the volume of the music
	///
	/// \copydetails AudioSource::setVolume
	///
	///////////////////////////////////////////////////////////
	void setVolume(float volume) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the music for spatial audio
	///
	/// \copydetails AudioSource::setPosition
	///
	///////////////////////////////////////////////////////////
	void setPosition(const Vector3f& pos) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the music for spatial audio
	///
	/// \copydetails AudioSource::setPosition(float,float,float)
	///
	///////////////////////////////////////////////////////////
	void setPosition(float x, float y, float z) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the minimum distance of the music for spatial audio
	///
	/// \copydetails AudioSource::setMinDistance
	///
	///////////////////////////////////////////////////////////
	void setMinDistance(float dist) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the atteunuation factor of the music for spatial audio
	///
	/// \copydetails AudioSource::setAttenuation
	///
	///////////////////////////////////////////////////////////
	void setAttenuation(float attenuation) override;

	///////////////////////////////////////////////////////////
	/// \brief Get the status of the music
	///
	/// \copydetails AudioSource::getStatus
	///
	///////////////////////////////////////////////////////////
	AudioSource::Status getStatus() const override;

	///////////////////////////////////////////////////////////
	/// \brief Check if the music is looped
	///
	/// \return True if the music is looped
	///
	///////////////////////////////////////////////////////////
	bool getLoop() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the playback offset of the music
	///
	/// \return The playback offset of the music
	///
	///////////////////////////////////////////////////////////
	Time getPlayingOffset() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the pitch of the music
	///
	/// \copydetails AudioSource::getPitch
	///
	///////////////////////////////////////////////////////////
	float getPitch() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the volume of the music
	///
	/// \copydetails AudioSource::getVolume
	///
	///////////////////////////////////////////////////////////
	float getVolume() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the position of the music when using spatial audio
	///
	/// \copydetails AudioSource::getPosition
	///
	///////////////////////////////////////////////////////////
	Vector3f getPosition() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the minimum distance of the music when using spatial audio
	///
	/// \copydetails AudioSource::getMinDistance
	///
	///////////////////////////////////////////////////////////
	float getMinDistance() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the attenuation factor of the music when using spatial audio
	///
	/// \copydetails AudioSource::getAttenuation
	///
	///////////////////////////////////////////////////////////
	float getAttenuation() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the duration of the music file
	///
	/// \return The duration of the music file
	///
	///////////////////////////////////////////////////////////
	Time getDuration() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of channels used by the music
	///
	/// \return The number of channels
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumChannels() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the sample rate used by the music
	///
	/// \return The sample rate
	///
	///////////////////////////////////////////////////////////
	Uint32 getSampleRate() const;

private:
	priv::SfmlMusic m_music;		//!< The internal SFML music object
	sf::InputSoundFile m_file;		//!< The internal SFML input sound file
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Music
/// \ingroup Audio
///
/// The difference between Music and Sound are that instead of
/// completely loading audio data in memory, data is streamed
/// directly from a file. This allows longer audio files to be
/// played effectively, as uncompressed audio takes up quite a
/// bit of space.
///
/// To load and play an audio file, open an audio file with an
/// accepted file format using open(), then use the play() function
/// to begin playback. To use Music as a read stream, use the read()
/// function after opening a file. Note that the read() function,
/// like with all other read streams, handles data in bytes, not
/// samples. After reading data from the music stream, the playback
/// offset will be advanced by however much data was read. To
/// pipe music data, connect it to another write stream with
/// pipe(), and data will be pushed into that stream automatically
/// as the music is played. To transfer data as quick possible,
/// the write stream has to be pulling data from the music stream
/// as the music stream's pushing speed limit is the playback speed.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Music music;
/// music.open("music.ogg");
/// music.play();
///
/// // Wait some time...
///
/// music.stop();
///
/// // Stream the rest into a file writer
/// AudioFileWriter writer;
/// writer.open("music.flac");
/// music.pipe(&writer);
/// writer.save();
/// writer.close();
///
/// \endcode
///
///////////////////////////////////////////////////////////