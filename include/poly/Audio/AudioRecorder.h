#ifndef POLY_AUDIO_RECORDER_H
#define POLY_AUDIO_RECORDER_H

#include <poly/Core/Stream.h>
#include <poly/Core/Time.h>

#include <SFML/Audio/SoundRecorder.hpp>

#include <mutex>

namespace poly
{

class AudioRecorder;

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


///////////////////////////////////////////////////////////
class AudioRecorder :
	public ReadStream
{
	friend priv::SfmlAudioRecorder;

public:
	AudioRecorder();

	virtual ~AudioRecorder();

	Uint32 read(void* read, Uint32 max) override;

	bool start(Uint32 sampleRate = 44100);

	void stop();

	bool setDevice(const std::string& device);

	void setNumChannels(Uint32 numChannels);

	void setProcessingInterval(Time interval);

	bool isRecording() const;

	const std::string& getDevice() const;

	Uint32 getNumChannels() const;

	Uint32 getSampleRate() const;

	static bool isAvailable();

	static std::string getDefaultDevice();

	static std::vector<std::string> getAvailableDevices();

private:
	priv::SfmlAudioRecorder m_recorder;
	RingBuffer m_buffer;
	std::mutex m_mutex;
	bool m_isRecording;
};


}

#endif