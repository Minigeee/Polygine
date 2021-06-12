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
	if (!m_isInitialized)
	{
		initialize(numChannels, sampleRate);

		// Allocate enough space to store 1 second of audio
		m_buffer = (Int16*)MALLOC_DBG(sampleRate * numChannels * 2);

		m_isInitialized = true;
	}
}


///////////////////////////////////////////////////////////
bool SfmlAudioStream::onGetData(sf::SoundStream::Chunk& chunk)
{
	// Read data from stream
	Uint32 numSamples = getSampleRate() * getChannelCount();
	numSamples = m_stream->read(m_buffer, numSamples);

	// Set chunk info
	chunk.samples = m_buffer;
	chunk.sampleCount = numSamples;

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
	m_stream		(this)
{

}


///////////////////////////////////////////////////////////
AudioStream::~AudioStream()
{

}


///////////////////////////////////////////////////////////
Uint32 AudioStream::read(Int16* samples, Uint32 max)
{
	// Read from buffer stream
	return BufferStream::read(samples, max * 2) / 2;
}


///////////////////////////////////////////////////////////
void AudioStream::pipe(WriteStream* stream)
{
	m_pipe = stream;

	Uint32 maxSize = m_sampleRate * m_numChannels * 2;

	// Send all data
	while (m_size)
	{
		// Send data in batches of 1 sec of audio
		Uint32 batchSize = m_size > maxSize ? maxSize : m_size;

		Uint32 offset = (Uint8*)m_front - (Uint8*)m_buffer;
		Uint32 newOffset = (offset + batchSize) % m_capacity;
		Uint32 roffset = m_capacity - offset;

		if (roffset < batchSize)
		{
			// Handle end of ring queue
			stream->write(m_front, roffset);
			stream->write(m_buffer, batchSize - roffset);

			if (m_onReadData)
			{
				m_onReadData(m_front, roffset);
				m_onReadData(m_buffer, batchSize - roffset);
			}
		}
		else
		{
			stream->write(m_front, batchSize);
			if (m_onReadData)
				m_onReadData(m_front, batchSize);
		}

		// Update properties
		m_size -= batchSize;
		m_front = (Uint8*)m_buffer + newOffset;
	}
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
void AudioStream::onSeek(Time offset)
{
	// Normal streams can't seek
}


}