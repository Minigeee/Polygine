#ifndef POLY_AUDIO_SOURCE_H
#define POLY_AUDIO_SOURCE_H

#include <poly/Math/Vector3.h>

namespace poly
{


class AudioSource
{
public:
	enum Status
	{
		Stopped,
		Paused,
		Playing
	};

public:
	virtual ~AudioSource() { }

	virtual void play() = 0;

	virtual void stop() = 0;

	virtual void setPitch(float pitch) = 0;

	virtual void setVolume(float volume) = 0;

	virtual void setPosition(const Vector3f& pos) = 0;

	virtual void setPosition(float x, float y, float z) = 0;

	virtual void setMinDistance(float dist) = 0;

	virtual void setAttenuation(float attenuation) = 0;

	virtual Status getStatus() const = 0;

	virtual float getPitch() const = 0;

	virtual float getVolume() const = 0;

	virtual Vector3f getPosition() const = 0;

	virtual float getMinDistance() const = 0;

	virtual float getAttenuation() const = 0;
};


}

#endif