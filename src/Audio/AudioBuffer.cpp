#include <poly/Audio/AudioBuffer.h>

namespace poly
{


///////////////////////////////////////////////////////////
Uint32 AudioBuffer::read(Int16* samples, Uint32 max)
{
	// Get number of samples
	Uint32 numSamples = (Uint32)m_buffer.getSampleCount();
	if (numSamples > max)
		numSamples = max;

	// Always return data starting from the beggining
	memcpy(samples, m_buffer.getSamples(), numSamples * 2);

	// Return number of samples copied
	return numSamples;
}


///////////////////////////////////////////////////////////
bool AudioBuffer::load(const std::string& fname)
{
	// Load from file
	if (!m_buffer.loadFromFile(fname))
		return false;

	// Set parameters
	m_numChannels = m_buffer.getChannelCount();
	m_sampleRate = m_buffer.getSampleRate();

	return true;
}


///////////////////////////////////////////////////////////
bool AudioBuffer::setSamples(Int16* samples, Uint32 num, Uint32 numChannels, Uint32 sampleRate)
{
	// Load from samples
	if (!m_buffer.loadFromSamples(samples, num, numChannels, sampleRate))
		return false;

	// Set parameters
	m_numChannels = m_buffer.getChannelCount();
	m_sampleRate = m_buffer.getSampleRate();

	return true;
}


///////////////////////////////////////////////////////////
Uint32 AudioBuffer::getNumSamples() const
{
	return (Uint32)m_buffer.getSampleCount();
}


}