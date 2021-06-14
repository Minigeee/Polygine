#include <poly/Audio/AudioFileWriter.h>

#include <poly/Core/Allocate.h>

namespace poly
{


///////////////////////////////////////////////////////////
AudioFileWriter::AudioFileWriter() :
	m_sampleRate			(44100),
	m_numChannels			(1),
	m_isOpen				(false)
{

}


///////////////////////////////////////////////////////////
bool AudioFileWriter::open(const std::string& fname, Uint32 sampleRate, Uint32 numChannels)
{
	if (!m_file.openFromFile(fname, sampleRate, numChannels))
		return false;

	m_sampleRate = sampleRate;
	m_numChannels = numChannels;
	m_isOpen = true;

	return true;
}


///////////////////////////////////////////////////////////
void AudioFileWriter::close()
{
	m_file.close();
	m_isOpen = false;
}


///////////////////////////////////////////////////////////
Uint32 AudioFileWriter::write(void* data, Uint32 num)
{
	if (!m_isOpen) return 0;

	std::unique_lock<std::mutex> lock(m_mutex);
	m_file.write((Int16*)data, (Uint64)(num / 2));

	return num;
}


///////////////////////////////////////////////////////////
void AudioFileWriter::write(Int16* samples, Uint32 num)
{
	if (!m_isOpen) return;

	std::unique_lock<std::mutex> lock(m_mutex);
	m_file.write(samples, (Uint64)num);
}


///////////////////////////////////////////////////////////
void AudioFileWriter::save(ReadStream* input)
{
	if (!m_isOpen) return;

	std::unique_lock<std::mutex> lock(m_mutex);

	// Read from stream in chunks of 1 sec
	Uint32 numSamples = m_sampleRate * m_numChannels;
	Int16* samples = (Int16*)MALLOC_DBG(numSamples * 2);

	Uint32 numRead = 0;
	while (numRead = input->read(samples, numSamples * 2))
		// Write to file
		m_file.write(samples, numRead / 2);

	// Free temp buffer
	FREE_DBG(samples);
}


///////////////////////////////////////////////////////////
void AudioFileWriter::save()
{
	// Save all input streams
	for (Uint32 i = 0; i < m_inputs.size(); ++i)
		save(m_inputs[i]);
}


///////////////////////////////////////////////////////////
bool AudioFileWriter::isOpen() const
{
	return m_isOpen;
}


}