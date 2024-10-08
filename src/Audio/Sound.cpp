#include <poly/Audio/AudioBuffer.h>
#include <poly/Audio/Sound.h>

namespace poly
{


///////////////////////////////////////////////////////////
Sound::Sound() :
	m_buffer		(0)
{

}


///////////////////////////////////////////////////////////
void Sound::play()
{
	m_sound.play();
}


///////////////////////////////////////////////////////////
void Sound::pause()
{
	m_sound.pause();
}


///////////////////////////////////////////////////////////
void Sound::stop()
{
	m_sound.stop();
}


///////////////////////////////////////////////////////////
void Sound::setBuffer(AudioBuffer* buffer)
{
	m_buffer = buffer;
	m_sound.setBuffer(buffer->m_buffer);
}


///////////////////////////////////////////////////////////
void Sound::setLoop(bool loop)
{
	m_sound.setLoop(loop);
}


///////////////////////////////////////////////////////////
void Sound::setPlayingOffset(Time offset)
{
	m_sound.setPlayingOffset(sf::microseconds(offset.toMicroseconds()));
}


///////////////////////////////////////////////////////////
void Sound::setPitch(float pitch)
{
	m_sound.setPitch(pitch);
}


///////////////////////////////////////////////////////////
void Sound::setVolume(float volume)
{
	m_sound.setVolume(volume);
}


///////////////////////////////////////////////////////////
void Sound::setPosition(const Vector3f& pos)
{
	m_sound.setPosition(pos.x, pos.y, pos.z);
}


///////////////////////////////////////////////////////////
void Sound::setPosition(float x, float y, float z)
{
	m_sound.setPosition(x, y, z);
}


///////////////////////////////////////////////////////////
void Sound::setMinDistance(float dist)
{
	m_sound.setMinDistance(dist);
}


///////////////////////////////////////////////////////////
void Sound::setAttenuation(float attenuation)
{
	m_sound.setAttenuation(attenuation);
}


///////////////////////////////////////////////////////////
AudioSource::Status Sound::getStatus() const
{
	return (Status)m_sound.getStatus();
}


///////////////////////////////////////////////////////////
AudioBuffer* Sound::getBuffer() const
{
	return m_buffer;
}


///////////////////////////////////////////////////////////
bool Sound::getLoop() const
{
	return m_sound.getLoop();
}


///////////////////////////////////////////////////////////
Time Sound::getPlayingOffset() const
{
	return Time::fromMicroseconds(m_sound.getPlayingOffset().asMicroseconds());
}


///////////////////////////////////////////////////////////
float Sound::getPitch() const
{
	return m_sound.getPitch();
}


///////////////////////////////////////////////////////////
float Sound::getVolume() const
{
	return m_sound.getVolume();
}


///////////////////////////////////////////////////////////
Vector3f Sound::getPosition() const
{
	sf::Vector3f pos = m_sound.getPosition();
	return Vector3f(pos.x, pos.y, pos.z);
}


///////////////////////////////////////////////////////////
float Sound::getMinDistance() const
{
	return m_sound.getMinDistance();
}


///////////////////////////////////////////////////////////
float Sound::getAttenuation() const
{
	return m_sound.getAttenuation();
}


}