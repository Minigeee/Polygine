#include <poly/Audio/AudioStream.h>

#include <poly/Core/Allocate.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
SfmlAudioStream::SfmlAudioStream(AudioStream* stream) :
	m_stream		(stream),
	m_buffer		(0),
	m_isInitialized	(false)
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
void SfmlAudioStream::init(Uint32 numChannels, Uint32 sampleRate)
{
	if (getChannelCount() != numChannels || getSampleRate() != sampleRate)
	{
		stop();
		initialize(numChannels, sampleRate);

		// Allocate enough space to store 1 second of audio
		m_buffer = (Int16*)MALLOC_DBG(sampleRate * numChannels * 2);
	}
}


///////////////////////////////////////////////////////////
bool SfmlAudioStream::onGetData(sf::SoundStream::Chunk& chunk)
{
	// Read data from stream
	Uint32 numBytes = getSampleRate() * getChannelCount() * 2;
	numBytes = m_stream->read(m_buffer, numBytes);

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
	m_sampleRate		(44100)
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
	m_stream.init(m_numChannels, m_sampleRate);
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