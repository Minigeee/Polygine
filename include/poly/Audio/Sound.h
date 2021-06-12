#ifndef POLY_SOUND_H
#define POLY_SOUND_H

#include <poly/Audio/AudioSource.h>

#include <poly/Core/Time.h>

#include <SFML/Audio/Sound.hpp>

namespace poly
{


class AudioBuffer;


class Sound : public AudioSource
{
public:
	Sound();

	void play() override;

	void pause();

	void stop() override;

	void setBuffer(AudioBuffer* buffer);

	void setLoop(bool loop);

	void setPlayingOffset(Time offset);

	void setPitch(float pitch) override;

	void setVolume(float volume) override;

	void setPosition(const Vector3f& pos) override;

	void setPosition(float x, float y, float z) override;

	void setMinDistance(float dist) override;

	void setAttenuation(float attenuation) override;

	AudioSource::Status getStatus() const override;

	AudioBuffer* getBuffer() const;

	bool getLoop() const;

	Time getPlayingOffset() const;

	float getPitch() const override;

	float getVolume() const override;

	Vector3f getPosition() const override;

	float getMinDistance() const override;

	float getAttenuation() const override;

private:
	sf::Sound m_sound;
	AudioBuffer* m_buffer;
};


}

#endif