#ifndef POLY_AUDIO_DATA_H
#define POLY_AUDIO_DATA_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/Stream.h>

namespace poly
{


class AudioData
{
public:
	AudioData();

	virtual ~AudioData();

	virtual Uint32 read(Int16* samples, Uint32 max) = 0;

	Uint32 getNumChannels() const;

	Uint32 getSampleRate() const;

protected:
	Uint32 m_numChannels;
	Uint32 m_sampleRate;
};


}

#endif