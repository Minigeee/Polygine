#include <poly/Audio/AudioBuffer.h>

namespace poly
{


///////////////////////////////////////////////////////////
AudioBuffer::AudioBuffer() :
	m_offset		(0)
{

}


///////////////////////////////////////////////////////////
Uint32 AudioBuffer::read(void* data, Uint32 max)
{
	// Get number of bytes
	Uint32 numBytes = (Uint32)m_buffer.getSampleCount() * 2 - m_offset * 2;
	if (numBytes > max)
		numBytes = max;

	// Always return data starting from the beggining
	memcpy(data, m_buffer.getSamples() + m_offset, numBytes);

	// Return number of bytes copied
	return numBytes;
}


///////////////////////////////////////////////////////////
Uint32 AudioBuffer::read(Int16* samples, Uint32 max, Uint32 offset)
{
	seek(offset);
	return ReadStream::read(samples, max * 2) / 2;
}


///////////////////////////////////////////////////////////
void AudioBuffer::seek(Uint32 offset)
{
	m_offset = 0;
}


///////////////////////////////////////////////////////////
bool AudioBuffer::load(const std::string& fname)
{
	// Load from file
	if (!m_buffer.loadFromFile(fname))
		return false;

	return true;
}


///////////////////////////////////////////////////////////
bool AudioBuffer::setSamples(Int16* samples, Uint32 num, Uint32 numChannels, Uint32 sampleRate)
{
	// Load from samples
	if (!m_buffer.loadFromSamples(samples, num, numChannels, sampleRate))
		return false;

	return true;
}


///////////////////////////////////////////////////////////
Uint32 AudioBuffer::getNumSamples() const
{
	return (Uint32)m_buffer.getSampleCount();
}


///////////////////////////////////////////////////////////
Uint32 AudioBuffer::getNumChannels() const
{
	return m_buffer.getChannelCount();
}


///////////////////////////////////////////////////////////
Uint32 AudioBuffer::getSampleRate() const
{
	return m_buffer.getSampleRate();
}


}