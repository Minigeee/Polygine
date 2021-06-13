#include <poly/Audio/AudioRecorder.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
SfmlAudioRecorder::SfmlAudioRecorder(AudioRecorder* recorder) :
	m_recorder			(recorder)
{

}


///////////////////////////////////////////////////////////
bool SfmlAudioRecorder::onProcessSamples(const Int16* samples, std::size_t num)
{
	// Write to stream
	m_recorder->write(const_cast<Int16*>(samples), num * 2);

	return true;
}


}


///////////////////////////////////////////////////////////
AudioRecorder::AudioRecorder() :
	m_recorder		(this)
{

}


///////////////////////////////////////////////////////////
AudioRecorder::~AudioRecorder()
{

}


///////////////////////////////////////////////////////////
bool AudioRecorder::start(Uint32 sampleRate)
{
	if (!m_recorder.start(sampleRate))
		return false;

	m_isRecording = true;

	return true;
}


///////////////////////////////////////////////////////////
void AudioRecorder::stop()
{
	m_recorder.stop();
	m_isRecording = false;
}


///////////////////////////////////////////////////////////
bool AudioRecorder::setDevice(const std::string& device)
{
	return m_recorder.setDevice(device);
}


///////////////////////////////////////////////////////////
void AudioRecorder::setNumChannels(Uint32 numChannels)
{
	m_recorder.setChannelCount(numChannels);
}


///////////////////////////////////////////////////////////
bool AudioRecorder::isRecording() const
{
	return m_isRecording;
}


///////////////////////////////////////////////////////////
Uint32 AudioRecorder::getNumChannels() const
{
	return m_recorder.getChannelCount();
}


///////////////////////////////////////////////////////////
Uint32 AudioRecorder::getSampleRate() const
{
	return m_recorder.getSampleRate();
}


///////////////////////////////////////////////////////////
const std::string& AudioRecorder::getDevice() const
{
	return m_recorder.getDevice();
}


///////////////////////////////////////////////////////////
bool AudioRecorder::isAvailable()
{
	return sf::SoundRecorder::isAvailable();
}


///////////////////////////////////////////////////////////
std::string AudioRecorder::getDefaultDevice()
{
	return sf::SoundRecorder::getDefaultDevice();
}


///////////////////////////////////////////////////////////
std::vector<std::string> AudioRecorder::getAvailableDevices()
{
	return sf::SoundRecorder::getAvailableDevices();
}


}