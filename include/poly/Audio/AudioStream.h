#ifndef POLY_AUDIO_STREAM_H
#define POLY_AUDIO_STREAM_H

#include <poly/Audio/AudioSource.h>

#include <poly/Core/Stream.h>
#include <poly/Core/Time.h>

#include <SFML/Audio/SoundStream.hpp>

#include <mutex>

namespace poly
{

class AudioStream;

#ifndef DOXYGEN_SKIP
namespace priv
{


///////////////////////////////////////////////////////////
class SfmlAudioStream : public sf::SoundStream
{
public:
	SfmlAudioStream(AudioStream* stream);

	~SfmlAudioStream();

	void init(Uint32 numChannels, Uint32 sampleRate, Time interval);

private:
	bool onGetData(Chunk& data) override;

	void onSeek(sf::Time timeOffset);

private:
	AudioStream* m_stream;
	Int16* m_buffer;
	Uint32 m_bufferSize;
	Time m_updateInterval;
};


}
#endif


///////////////////////////////////////////////////////////
/// \brief An audio source that has its audio data streamed into it
///
///////////////////////////////////////////////////////////
class AudioStream :
	public AudioSource,
	public WriteStream
{
	friend priv::SfmlAudioStream;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	AudioStream();

	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~AudioStream();

	///////////////////////////////////////////////////////////
	/// \brief Start playback of the audio stream
	///
	/// This function starts playback of the audio stream, where it
	/// will begin pulling data from its internal buffer first, then
	/// from any input read streams that pipe their data into the
	/// audio stream. Data will be pulled from the input streams in
	/// the order they were connected.
	///
	///////////////////////////////////////////////////////////
	virtual void play() override;

	///////////////////////////////////////////////////////////
	/// \brief Pause playback of the audio stream
	///
	/// For a default audio stream, this function is equivalent
	/// to stop(), as there is no playback position in a default
	/// audio stream. Playback is stopped until play() is called
	/// again.
	///
	///////////////////////////////////////////////////////////
	virtual void pause();

	///////////////////////////////////////////////////////////
	/// \brief Sstop playback of the audio stream
	///
	/// For a default audio stream, this function is equivalent
	/// to pause(), as there is no playback position in a default
	/// audio stream. However the stream will attempt to reset playback
	/// position to the beggining for any derived classes that choose
	/// to implement a seek function. Playback is stopped until play()
	/// is called again.
	///
	///////////////////////////////////////////////////////////
	virtual void stop() override;

	///////////////////////////////////////////////////////////
	/// \brief Write audio sample data to be played in the audio stream
	///
	/// Any amount of sample data can be written into the stream.
	/// The data will be stored in a ring buffer until it is requested
	/// by the stream. The stream will remove the data as it is played.
	/// Like with any stream, the data is handled in bytes, which is
	/// half the size of an audio sample, so keep this in mind when
	/// setting the size of the data.
	///
	/// \param data A pointer to the data that will be written
	/// \param size The number of bytes to write into the stream
	///
	/// \return The number of bytes that was actually written into the stream
	///
	///////////////////////////////////////////////////////////
	virtual Uint32 write(const void* data, Uint32 size) override;

	///////////////////////////////////////////////////////////
	/// \brief Clear all data from the internal ring buffer
	///
	///////////////////////////////////////////////////////////
	void flush();

	///////////////////////////////////////////////////////////
	/// \brief Set the interval at which the stream requests more data
	///
	/// Changing this value to a smaller interval value is necessary
	/// for more 'real-time' playback of audio data, as it will minimize
	/// the time needed to completely fill the interval with data.
	///
	/// \param interval the update interval
	///
	///////////////////////////////////////////////////////////
	void setUpdateInterval(Time interval);

	///////////////////////////////////////////////////////////
	/// \brief Set the number of channels to use while playing the streamed data
	///
	/// \param channels The number of channels
	///
	///////////////////////////////////////////////////////////
	void setNumChannels(Uint32 channels);

	///////////////////////////////////////////////////////////
	/// \brief Set the sample rate to use when playing the streamed data
	///
	/// \param rate The sample rate
	///
	///////////////////////////////////////////////////////////
	void setSampleRate(Uint32 rate);

	///////////////////////////////////////////////////////////
	/// \brief Set the pitch of the audio stream
	///
	/// \copydetails AudioSource::setPitch
	///
	///////////////////////////////////////////////////////////
	void setPitch(float pitch) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the volume of the audio stream
	///
	/// \copydetails AudioSource::setVolume
	///
	///////////////////////////////////////////////////////////
	void setVolume(float volume) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the audio stream for spatial audio
	///
	/// \copydetails AudioSource::setPosition
	///
	///////////////////////////////////////////////////////////
	void setPosition(const Vector3f& pos) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the audio stream for spatial audio
	///
	/// \copydetails AudioSource::setPosition(float,float,float)
	///
	///////////////////////////////////////////////////////////
	void setPosition(float x, float y, float z) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the minimum distance of the audio stream for spatial audio
	///
	/// \copydetails AudioSource::setMinDistance
	///
	///////////////////////////////////////////////////////////
	void setMinDistance(float dist) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the atteunuation factor of the audio stream for spatial audio
	///
	/// \copydetails AudioSource::setAttenuation
	///
	///////////////////////////////////////////////////////////
	void setAttenuation(float attenuation) override;

	///////////////////////////////////////////////////////////
	/// \brief Get the status of the audio stream
	///
	/// \copydetails AudioSource::getStatus
	///
	///////////////////////////////////////////////////////////
	AudioSource::Status getStatus() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the pitch of the audio stream
	///
	/// \copydetails AudioSource::getPitch
	///
	///////////////////////////////////////////////////////////
	float getPitch() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the volume of the audio stream
	///
	/// \copydetails AudioSource::getVolume
	///
	///////////////////////////////////////////////////////////
	float getVolume() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the position of the audio stream when using spatial audio
	///
	/// \copydetails AudioSource::getPosition
	///
	///////////////////////////////////////////////////////////
	Vector3f getPosition() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the minimum distance of the audio stream when using spatial audio
	///
	/// \copydetails AudioSource::getMinDistance
	///
	///////////////////////////////////////////////////////////
	float getMinDistance() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the attenuation factor of the audio stream when using spatial audio
	///
	/// \copydetails AudioSource::getAttenuation
	///
	///////////////////////////////////////////////////////////
	float getAttenuation() const override;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of channels used by the audio stream
	///
	/// \return The number of channels
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumChannels() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the sample rate used by the audio stream
	///
	/// \return The sample rate
	///
	///////////////////////////////////////////////////////////
	Uint32 getSampleRate() const;

protected:
	///////////////////////////////////////////////////////////
	/// \brief This function is called when the playback position of the stream is attempted to be changed
	///
	/// Classes that override this function should make any changes
	/// that are necessary to change the playback position of the stream
	/// in this function. For example, for music, this function would
	/// change the playback offset of the input sound file.
	///
	/// \param offset The playback offset to seek to
	///
	///////////////////////////////////////////////////////////
	virtual void onSeek(Time offset);

protected:
	Uint32 m_numChannels;				//!< The number of channels being used for playback
	Uint32 m_sampleRate;				//!< The sample rate being used for playback
	priv::SfmlAudioStream m_stream;		//!< The internal SFML audio stream
	RingBuffer m_buffer;				//!< The buffer used to temporarily store audio data
	Time m_updateInterval;				//!< The update interval of the stream
	std::mutex m_mutex;					//!< A mutex used to protect writing and reading of the audio buffer
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::AudioStream
/// \ingroup Audio
///
/// An audio stream plays audio data that is passed into it in
/// real-time. This audio source should be used to play audio
/// data that needs real-time flexibilty that Music does not
/// offer, such as applying audio effects, or playing audio
/// data recieved over the network.
///
/// To use the audio stream, first set the desired number of channels
/// to use, and the desired sample rate. Then start writing data
/// into the stream by piping another read stream into it with
/// WriteStream::pipe(), or by writing data directly with write().
/// The audio stream will start playing as soon as it has enough data
/// to fill the equivalent of one update interval of audio samples.
/// At this point, make sure the fill rate of the audio stream is
/// equal to the playback speed of the stream so that the audio
/// stream never has to wait for new data, and so that the internal
/// stream buffer doesn't grow too large in size.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Immediate playback of audio recorded by a capture device
/// AudioRecorder recorder;
/// AudioStream stream;
///
/// // Set the desired number of channels and sample rate (this must match the data coming from the recorder)
/// stream.setNumChannels(1);
/// stream.setSampleRate(44100);
/// recorder.pipe(&stream);
///
/// // Set a low processing and update interval for more real-time audio playback
/// stream.setUpdateInterval(Time::fromMilliseconds(20));
/// recorder.setProcessingInterval(Time::fromMilliseconds(20));
///
/// // Start recording
/// recorder.start();
///
/// // Wait for however long...
///
/// recorder.stop();
///
/// \endcode
///
///////////////////////////////////////////////////////////