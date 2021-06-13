#ifndef POLY_AUDIO_RECORDER_H
#define POLY_AUDIO_RECORDER_H

#include <poly/Core/Stream.h>

#include <SFML/Audio/SoundRecorder.hpp>

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

private:
	bool onProcessSamples(const Int16* samples, std::size_t num) override;

private:
	AudioRecorder* m_recorder;
};


}


///////////////////////////////////////////////////////////
class AudioRecorder :
	public BufferStream
{
	friend priv::SfmlAudioRecorder;

public:
	AudioRecorder();

	virtual ~AudioRecorder();

	bool start(Uint32 sampleRate = 44100);

	void stop();

	bool setDevice(const std::string& device);

	void setNumChannels(Uint32 numChannels);

	bool isRecording() const;

	const std::string& getDevice() const;

	Uint32 getNumChannels() const;

	Uint32 getSampleRate() const;

	static bool isAvailable();

	static std::string getDefaultDevice();

	static std::vector<std::string> getAvailableDevices();

private:
	priv::SfmlAudioRecorder m_recorder;
	bool m_isRecording;
};


}

#endif