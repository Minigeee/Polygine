#ifndef POLY_AUDIO_H
#define POLY_AUDIO_H

#include <poly/Audio/Components.h>

#include <poly/Core/HandleArray.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Engine/Extension.h>

namespace poly
{


class AudioSource;


class Audio : public Extension
{
public:
	Audio(Scene* scene);

	~Audio();

	void update();

	Handle play(AudioSource* source);

	void stop(Handle handle);

	void setPitch(Handle handle, float pitch);

	void setVolume(Handle handle, float volume);

	AudioStatus getStatus(Handle handle) const;

	float getPitch(Handle handle) const;

	float getVolume(Handle handle) const;

private:
	ObjectPool m_streamPool;
	HandleArray<void*> m_streamData;
};


}

#endif