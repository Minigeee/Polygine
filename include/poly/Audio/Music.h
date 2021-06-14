#ifndef POLY_MUSIC_H
#define POLY_MUSIC_H

#include <poly/Audio/AudioSource.h>

#include <poly/Core/Stream.h>
#include <poly/Core/Time.h>

#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/Audio/SoundStream.hpp>

namespace poly
{

class Music;

namespace priv
{


///////////////////////////////////////////////////////////
class SfmlMusic : public sf::SoundStream
{
public:
	SfmlMusic(Music* music);

	~SfmlMusic();

	void init(Uint32 numChannels, Uint32 sampleRate);

private:
	bool onGetData(Chunk& data) override;

	void onSeek(sf::Time timeOffset);

private:
	Music* m_music;
	Int16* m_buffer;
};


}


///////////////////////////////////////////////////////////
class Music :
	public AudioSource,
	public ReadStream
{
	friend priv::SfmlMusic;

public:
	Music();

	~Music();

	bool open(const std::string& fname);

	void play() override;

	void pause();

	void stop() override;

	Uint32 read(void* read, Uint32 max) override;

	void setPitch(float pitch) override;

	void setVolume(float volume) override;

	void setPosition(const Vector3f& pos) override;

	void setPosition(float x, float y, float z) override;

	void setMinDistance(float dist) override;

	void setAttenuation(float attenuation) override;

	void setPlayingOffset(Time offset);

	void setLoop(bool loop);

	AudioSource::Status getStatus() const override;

	float getPitch() const override;

	float getVolume() const override;

	Vector3f getPosition() const override;

	float getMinDistance() const override;

	float getAttenuation() const override;

	Time getDuration() const;

	Time getPlayingOffset() const;

	bool getLoop() const;

	Uint32 getNumChannels() const;

	Uint32 getSampleRate() const;

private:
	priv::SfmlMusic m_music;
	sf::InputSoundFile m_file;
};


}

#endif