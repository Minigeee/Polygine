#ifndef POLY_AUDIO_STREAM_H
#define POLY_AUDIO_STREAM_H

#include <poly/Audio/AudioSource.h>

#include <poly/Core/Stream.h>
#include <poly/Core/Time.h>

#include <SFML/Audio/SoundStream.hpp>

namespace poly
{

class AudioStream;

namespace priv
{


///////////////////////////////////////////////////////////
class SfmlAudioStream : public sf::SoundStream
{
public:
	SfmlAudioStream(AudioStream* stream);

	~SfmlAudioStream();

	void init(Uint32 numChannels, Uint32 sampleRate);

private:
	bool onGetData(Chunk& data) override;

	void onSeek(sf::Time timeOffset);

private:
	AudioStream* m_stream;
	Int16* m_buffer;
	bool m_isInitialized;
};


}


///////////////////////////////////////////////////////////
class AudioStream :
	public AudioSource,
	public BufferStream
{
	friend priv::SfmlAudioStream;

public:
	AudioStream();

	virtual ~AudioStream();

	virtual void play() override;

	virtual void pause();

	virtual void stop() override;

	void setNumChannels(Uint32 channels);

	void setSampleRate(Uint32 rate);

	void setPitch(float pitch) override;

	void setVolume(float volume) override;

	void setPosition(const Vector3f& pos) override;

	void setPosition(float x, float y, float z) override;

	void setMinDistance(float dist) override;

	void setAttenuation(float attenuation) override;

	AudioSource::Status getStatus() const override;

	float getPitch() const override;

	float getVolume() const override;

	Vector3f getPosition() const override;

	float getMinDistance() const override;

	float getAttenuation() const override;

	Uint32 getNumChannels() const;

	Uint32 getSampleRate() const;

protected:
	virtual void onSeek(Time offset);

protected:
	Uint32 m_numChannels;
	Uint32 m_sampleRate;
	priv::SfmlAudioStream m_stream;
};


}

#endif