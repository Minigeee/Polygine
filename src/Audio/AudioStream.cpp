#include <poly/Audio/AudioStream.h>

#include <poly/Core/Allocate.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
SfmlAudioStream::SfmlAudioStream(AudioStream* stream) :
	m_stream			(stream),
	m_buffer			(0),
	m_bufferSize		(0)
{

}


///////////////////////////////////////////////////////////
SfmlAudioStream::~SfmlAudioStream()
{
	if (m_buffer)
		FREE_DBG(m_buffer);

	m_buffer = 0;
}


///////////////////////////////////////////////////////////
void SfmlAudioStream::init(Uint32 numChannels, Uint32 sampleRate, float bufferSize)
{
	if (getChannelCount() != numChannels || getSampleRate() != sampleRate)
	{
		stop();
		initialize(numChannels, sampleRate);

		// Allocate enough space to store 0.1 second of audio
		if (m_buffer)
			FREE_DBG(m_buffer);

		m_bufferSize = (Uint32)(sampleRate * numChannels * bufferSize);
		m_buffer = (Int16*)MALLOC_DBG(m_bufferSize);
	}
}


///////////////////////////////////////////////////////////
bool SfmlAudioStream::onGetData(sf::SoundStream::Chunk& chunk)
{
	// Read data from buffer
	Uint32 maxBytes = m_bufferSize;
	Uint32 numBytes = 0;
	{
		std::unique_lock<std::mutex> lock(m_stream->m_mutex);
		numBytes += m_stream->m_buffer.read(m_buffer, maxBytes);
	}

	// Pull from input streams if not enough data is present in the buffer
	std::vector<ReadStream*>& inputs = m_stream->m_inputs;
	for (Uint32 i = 0; i < inputs.size() && numBytes < maxBytes; ++i)
		numBytes += inputs[i]->read(m_buffer + (numBytes / 2), maxBytes - numBytes);

	// Set chunk info
	chunk.samples = m_buffer;
	chunk.sampleCount = numBytes / 2;

	// Return false if the number of read samples is zero
	return chunk.sampleCount != 0;
}


///////////////////////////////////////////////////////////
void SfmlAudioStream::onSeek(sf::Time offset)
{
	m_stream->onSeek(Time::fromMicroseconds(offset.asMicroseconds()));
}


}


///////////////////////////////////////////////////////////
AudioStream::AudioStream() :
	m_stream			(this),
	m_numChannels		(1),
	m_sampleRate		(44100),
	m_updateInterval	(Time::fromMilliseconds(100))
{

}


///////////////////////////////////////////////////////////
AudioStream::~AudioStream()
{

}


///////////////////////////////////////////////////////////
void AudioStream::play()
{
	// Initialize stream before playing
	m_stream.init(m_numChannels, m_sampleRate, m_updateInterval.toSeconds());
	m_stream.play();
}


///////////////////////////////////////////////////////////
void AudioStream::pause()
{
	m_stream.pause();
}


///////////////////////////////////////////////////////////
void AudioStream::stop()
{
	m_stream.stop();
}


///////////////////////////////////////////////////////////
Uint32 AudioStream::write(void* data, Uint32 size)
{
	// Write into buffer
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_buffer.write(data, size);
	}

	// Start playing if stopped and there is enough data
	m_stream.init(m_numChannels, m_sampleRate, m_updateInterval.toSeconds());
	Uint32 sampleThreshold = (Uint32)(m_updateInterval.toSeconds() * m_stream.getChannelCount() * m_stream.getSampleRate());
	if (m_stream.getStatus() == sf::SoundSource::Stopped && m_buffer.size() / 2 > sampleThreshold)
		m_stream.play();

	return size;
}


///////////////////////////////////////////////////////////
void AudioStream::flush()
{
	m_buffer.clear();
}


///////////////////////////////////////////////////////////
void AudioStream::setUpdateInterval(Time interval)
{
	m_updateInterval = interval;
}


///////////////////////////////////////////////////////////
void AudioStream::setNumChannels(Uint32 channels)
{
	m_numChannels = channels;
}


///////////////////////////////////////////////////////////
void AudioStream::setSampleRate(Uint32 rate)
{
	m_sampleRate = rate;
}


///////////////////////////////////////////////////////////
void AudioStream::setPitch(float pitch)
{
	m_stream.setPitch(pitch);
}


///////////////////////////////////////////////////////////
void AudioStream::setVolume(float volume)
{
	m_stream.setVolume(volume);
}


///////////////////////////////////////////////////////////
void AudioStream::setPosition(const Vector3f& pos)
{
	m_stream.setPosition(pos.x, pos.y, pos.z);
}


///////////////////////////////////////////////////////////
void AudioStream::setPosition(float x, float y, float z)
{
	m_stream.setPosition(x, y, z);
}


///////////////////////////////////////////////////////////
void AudioStream::setMinDistance(float dist)
{
	m_stream.setMinDistance(dist);
}


///////////////////////////////////////////////////////////
void AudioStream::setAttenuation(float attenuation)
{
	m_stream.setAttenuation(attenuation);
}


///////////////////////////////////////////////////////////
AudioSource::Status AudioStream::getStatus() const
{
	return (Status)m_stream.getStatus();
}


///////////////////////////////////////////////////////////
float AudioStream::getPitch() const
{
	return m_stream.getPitch();
}


///////////////////////////////////////////////////////////
float AudioStream::getVolume() const
{
	return m_stream.getVolume();
}


///////////////////////////////////////////////////////////
Vector3f AudioStream::getPosition() const
{
	sf::Vector3f pos = m_stream.getPosition();
	return Vector3f(pos.x, pos.y, pos.z);
}


///////////////////////////////////////////////////////////
float AudioStream::getMinDistance() const
{
	return m_stream.getMinDistance();
}


///////////////////////////////////////////////////////////
float AudioStream::getAttenuation() const
{
	return m_stream.getAttenuation();
}


///////////////////////////////////////////////////////////
Uint32 AudioStream::getNumChannels() const
{
	return m_numChannels;
}


///////////////////////////////////////////////////////////
Uint32 AudioStream::getSampleRate() const
{
	return m_sampleRate;
}


///////////////////////////////////////////////////////////
void AudioStream::onSeek(Time offset)
{
	// Normal streams can't seek
}


}