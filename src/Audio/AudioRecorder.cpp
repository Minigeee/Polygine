#include <poly/Audio/AudioRecorder.h>

#include <poly/Core/Logger.h>

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
void SfmlAudioRecorder::setInterval(sf::Time interval)
{
	setProcessingInterval(interval);
}


///////////////////////////////////////////////////////////
bool SfmlAudioRecorder::onProcessSamples(const Int16* samples, std::size_t num)
{
	// Write to output streams
	std::vector<WriteStream*>& outputs = m_recorder->m_outputs;
	for (Uint32 i = 0; i < outputs.size(); ++i)
		outputs[i]->write(samples, num * 2);

	// Write to buffer if no output streams available
	if (!outputs.size())
	{
		std::unique_lock<std::mutex> lock(m_recorder->m_mutex);
		m_recorder->m_buffer.write(samples, num * 2);
	}

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
Uint32 AudioRecorder::read(void* buffer, Uint32 max)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_buffer.read(buffer, max);
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
void AudioRecorder::flush()
{
	m_buffer.clear();
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
void AudioRecorder::setProcessingInterval(Time interval)
{
	if (m_isRecording)
		m_recorder.setInterval(sf::microseconds(interval.toMicroseconds()));
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