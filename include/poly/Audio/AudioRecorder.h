#ifndef POLY_AUDIO_RECORDER_H
#define POLY_AUDIO_RECORDER_H

#include <poly/Core/Stream.h>
#include <poly/Core/Time.h>

#include <SFML/Audio/SoundRecorder.hpp>

#include <mutex>

namespace poly
{

class AudioRecorder;

#ifndef DOXYGEN_SKIP
namespace priv
{


///////////////////////////////////////////////////////////
class SfmlAudioRecorder : public sf::SoundRecorder
{
public:
	SfmlAudioRecorder(AudioRecorder* recorder);

	void setInterval(sf::Time interval);

private:
	bool onProcessSamples(const Int16* samples, std::size_t num) override;

private:
	AudioRecorder* m_recorder;
};


}
#endif


///////////////////////////////////////////////////////////
/// \brief Record audio data through an audio capture device
///
///////////////////////////////////////////////////////////
class AudioRecorder :
	public ReadStream
{
	friend priv::SfmlAudioRecorder;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	AudioRecorder();

	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~AudioRecorder();

	///////////////////////////////////////////////////////////
	/// \brief Read audio samples from the recorder stream
	///
	/// \param buffer A pointer to the output buffer where audio sapmles will be copied into
	/// \param max The maximum number of bytes to read
	///
	/// \return The actual number of bytes that was read
	///
	///////////////////////////////////////////////////////////
	Uint32 read(void* buffer, Uint32 max) override;

	///////////////////////////////////////////////////////////
	/// \brief Start recording audio data through an audio capture device
	///
	/// \param sampleRate The sample rate to record audio at
	///
	/// \return True if the recorder successfully started recording audio data
	///
	///////////////////////////////////////////////////////////
	bool start(Uint32 sampleRate = 44100);

	///////////////////////////////////////////////////////////
	/// \brief Clear the internal ring buffer of all data
	///
	///////////////////////////////////////////////////////////
	void flush();

	///////////////////////////////////////////////////////////
	/// \brief Stop recording audio data
	///
	///////////////////////////////////////////////////////////
	void stop();

	///////////////////////////////////////////////////////////
	/// \brief Set the audio capture device
	///
	/// The audio capture device can be changed while recording.
	/// A list of available devices can be retreieved using
	/// getAvailableDevices().
	///
	/// \param device The string name of the device to use
	///
	/// \return True if the device was succesfully switched to
	///
	///////////////////////////////////////////////////////////
	bool setDevice(const std::string& device);

	///////////////////////////////////////////////////////////
	/// \brief Set the number of channels to use for audio recording
	///
	/// \param channels The number of channels to use for audio recording (1 or 2)
	///
	///////////////////////////////////////////////////////////
	void setNumChannels(Uint32 channels);

	///////////////////////////////////////////////////////////
	/// \brief Set the recording processing interval
	///
	/// This interval determine how often audio data is returned
	/// and made available for reading out of the record stream.
	/// The smaller the interval is, the more often audio data is
	/// made available and the more 'real-time' the audio is. By
	/// default, the interval is set to 100ms.
	///
	/// \param interval The processing interval
	///
	///////////////////////////////////////////////////////////
	void setProcessingInterval(Time interval);

	///////////////////////////////////////////////////////////
	/// \brief Check if audio data is being recorded
	///
	/// \return True if the audio capture device is active and recording data
	///
	///////////////////////////////////////////////////////////
	bool isRecording() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the string name of the current audio capture device
	///
	/// \return The string name of the audio capture device
	///
	///////////////////////////////////////////////////////////
	const std::string& getDevice() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of channels the audio capture device is using
	///
	/// \return The number of channels the device is using to record
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumChannels() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the sample rate the audio capture device is currently using to record
	///
	/// \return The sample rate of the audio capture device
	///
	///////////////////////////////////////////////////////////
	Uint32 getSampleRate() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the system supports audio capture
	///
	/// \return True if audio capture is supported
	///
	///////////////////////////////////////////////////////////
	static bool isAvailable();

	///////////////////////////////////////////////////////////
	/// \brief Get the string name of the system default audio capture device
	///
	/// \return The string name of default audio capture device
	///
	///////////////////////////////////////////////////////////
	static std::string getDefaultDevice();

	///////////////////////////////////////////////////////////
	/// \brief Get the list of available audio capture devices
	///
	/// \return A list iof available audio capture devices
	///
	///////////////////////////////////////////////////////////
	static std::vector<std::string> getAvailableDevices();

private:
	priv::SfmlAudioRecorder m_recorder;		//!< The internal SFML audio recorder
	RingBuffer m_buffer;					//!< A ring buffer to store recorded data
	std::mutex m_mutex;						//!< A mutex to handle multithreaded access to recorder/audio buffer
	bool m_isRecording;						//!< A boolean used to determine if the capture device is recording
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::AudioRecorder
/// \ingroup Audio
///
/// The audio recorder enables audio data to be recorded through
/// an audio capture device (a microphone). The audio data is
/// recorded in batches and made available to the used in time
/// intervalas that can be controlled with setProcessingInterval().
/// At the end of every interval, the next batch of recorded audio
/// data is made available to be read() from the record stream.
/// The default interval is 100ms.
///
/// All recorded data is stored in a ring buffer internally,
/// until it is read from the record stream. If this audio
/// record stream is piped into a write stream using pipe(),
/// the internal ring buffer isn't used and memory is never
/// allocated for storage of the audio data, but is instead
/// written directly into the write stream.
///
/// To start recording audio data, use the start() function,
/// passing in the desired sample rate. To stop recording, use
/// the stop() function. The audio capture device can be set
/// with setDevice() by passing in the string name of the desired
/// audio capture device, and the list of available devices can
/// be retrieved using the static function getAvailableDevices().
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a buffer for audio data
/// Int16 buffer[100000];
///
/// // Audio recorder
/// AudioRecorder recorder;
///
/// // Record audio for 1 second
/// recorder.start();
/// sleep(Time::fromMilliseconds(1000));
/// recorder.stop();
///
/// // Data can be accessed with read
/// recorder.read(buffer, 100000);
///
/// // Or it can be piped into a write stream
/// AudioFileWriter writer;
/// writer.open("voice.wav");
/// recorder.pipe(&writer);
///
/// // Record audio for 1 second
/// recorder.start();
/// sleep(Time::fromMilliseconds(1000));
/// recorder.stop();
///
/// writer.close();
///
/// // Audio should be saved in the file now
///
/// \endcode
///
///////////////////////////////////////////////////////////