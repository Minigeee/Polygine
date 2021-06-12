#include <poly/Audio/Music.h>

namespace poly
{


///////////////////////////////////////////////////////////
Music::Music()
{

}


///////////////////////////////////////////////////////////
Music::~Music()
{
	stop();
}


///////////////////////////////////////////////////////////
bool Music::open(const std::string& fname)
{
	stop();

	// Open file
	if (!m_file.openFromFile(fname))
		return false;

	// Initialization
	m_numChannels = m_file.getChannelCount();
	m_sampleRate = m_file.getSampleRate();
	m_stream.init(m_numChannels, m_sampleRate);

	return true;
}


///////////////////////////////////////////////////////////
Uint32 Music::read(Int16* samples, Uint32 max)
{
	// Read from buffer first
	Uint32 numRead = AudioStream::read(samples, max);
	Uint32 numToFill = max - numRead;

	if (!numToFill)
		return numRead;

	// Read the remaining from the file
	numRead += (Uint32)m_file.read(samples + numRead, numToFill);

	return numRead;
}


///////////////////////////////////////////////////////////
void Music::setPlayingOffset(Time offset)
{
	m_stream.setPlayingOffset(sf::microseconds(offset.toMicroseconds()));
}


///////////////////////////////////////////////////////////
void Music::setLoop(bool loop)
{
	m_stream.setLoop(loop);
}


///////////////////////////////////////////////////////////
Time Music::getDuration() const
{
	return Time::fromMicroseconds(m_file.getDuration().asMicroseconds());
}


///////////////////////////////////////////////////////////
Time Music::getPlayingOffset() const
{
	return Time::fromMicroseconds(m_stream.getPlayingOffset().asMicroseconds());
}


///////////////////////////////////////////////////////////
bool Music::getLoop() const
{
	return m_stream.getLoop();
}


///////////////////////////////////////////////////////////
void Music::onSeek(Time offset)
{
	// Change file offset
	m_file.seek(sf::microseconds(offset.toMicroseconds()));
}


}