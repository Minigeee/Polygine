#include <poly/Audio/Music.h>

#include <poly/Core/Allocate.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
SfmlMusic::SfmlMusic(Music* music) :
	m_buffer			(0),
	m_music				(music)
{

}


///////////////////////////////////////////////////////////
SfmlMusic::~SfmlMusic()
{
	if (m_buffer)
		FREE_DBG(m_buffer);

	m_buffer = 0;
}


///////////////////////////////////////////////////////////
void SfmlMusic::init(Uint32 numChannels, Uint32 sampleRate)
{
	if (getChannelCount() != numChannels || getSampleRate() != sampleRate)
	{
		stop();
		initialize(numChannels, sampleRate);

		// Allocate enough space to store 1 second of audio
		if (m_buffer)
			FREE_DBG(m_buffer);
		m_buffer = (Int16*)MALLOC_DBG(sampleRate * numChannels * 2);
	}
}


///////////////////////////////////////////////////////////
bool SfmlMusic::onGetData(sf::SoundStream::Chunk& chunk)
{
	// Read data from buffer
	Uint32 maxSamples = getSampleRate() * getChannelCount();
	Uint32 numSamples = (Uint32)m_music->m_file.read(m_buffer, maxSamples);

	// Push this data to any output streams
	if (numSamples)
	{
		std::vector<WriteStream*>& outputs = m_music->m_outputs;
		for (Uint32 i = 0; i < outputs.size(); ++i)
			outputs[i]->write(m_buffer, numSamples * 2);
	}

	// Set chunk info
	chunk.samples = m_buffer;
	chunk.sampleCount = numSamples;

	// Return false if the number of read samples is zero
	return chunk.sampleCount != 0;
}


///////////////////////////////////////////////////////////
void SfmlMusic::onSeek(sf::Time offset)
{
	m_music->m_file.seek(offset);
}


}


///////////////////////////////////////////////////////////
Music::Music() :
	m_music			(this)
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
	m_music.init(m_file.getChannelCount(), m_file.getSampleRate());

	return true;
}


///////////////////////////////////////////////////////////
void Music::play()
{
	m_music.play();
}


///////////////////////////////////////////////////////////
void Music::pause()
{
	m_music.pause();
}


///////////////////////////////////////////////////////////
void Music::stop()
{
	m_music.stop();
}


///////////////////////////////////////////////////////////
Uint32 Music::read(void* samples, Uint32 max)
{
	// Read from the file
	return (Uint32)m_file.read((Int16*)samples, max / 2) * 2;
}


///////////////////////////////////////////////////////////
void Music::setPitch(float pitch)
{
	m_music.setPitch(pitch);
}


///////////////////////////////////////////////////////////
void Music::setVolume(float volume)
{
	m_music.setVolume(volume);
}


///////////////////////////////////////////////////////////
void Music::setPosition(const Vector3f& pos)
{
	m_music.setPosition(pos.x, pos.y, pos.z);
}


///////////////////////////////////////////////////////////
void Music::setPosition(float x, float y, float z)
{
	m_music.setPosition(x, y, z);
}


///////////////////////////////////////////////////////////
void Music::setMinDistance(float dist)
{
	m_music.setMinDistance(dist);
}


///////////////////////////////////////////////////////////
void Music::setAttenuation(float attenuation)
{
	m_music.setAttenuation(attenuation);
}


///////////////////////////////////////////////////////////
void Music::setPlayingOffset(Time offset)
{
	m_music.setPlayingOffset(sf::microseconds(offset.toMicroseconds()));
}


///////////////////////////////////////////////////////////
void Music::setLoop(bool loop)
{
	m_music.setLoop(loop);
}


///////////////////////////////////////////////////////////
AudioSource::Status Music::getStatus() const
{
	return (Status)m_music.getStatus();
}


///////////////////////////////////////////////////////////
float Music::getPitch() const
{
	return m_music.getPitch();
}


///////////////////////////////////////////////////////////
float Music::getVolume() const
{
	return m_music.getVolume();
}


///////////////////////////////////////////////////////////
Vector3f Music::getPosition() const
{
	sf::Vector3f pos = m_music.getPosition();
	return Vector3f(pos.x, pos.y, pos.z);
}


///////////////////////////////////////////////////////////
float Music::getMinDistance() const
{
	return m_music.getMinDistance();
}


///////////////////////////////////////////////////////////
float Music::getAttenuation() const
{
	return m_music.getAttenuation();
}


///////////////////////////////////////////////////////////
Uint32 Music::getNumChannels() const
{
	return m_music.getChannelCount();
}


///////////////////////////////////////////////////////////
Uint32 Music::getSampleRate() const
{
	return m_music.getSampleRate();
}


///////////////////////////////////////////////////////////
Time Music::getDuration() const
{
	return Time::fromMicroseconds(m_file.getDuration().asMicroseconds());
}


///////////////////////////////////////////////////////////
Time Music::getPlayingOffset() const
{
	return Time::fromMicroseconds(m_music.getPlayingOffset().asMicroseconds());
}


///////////////////////////////////////////////////////////
bool Music::getLoop() const
{
	return m_music.getLoop();
}


}