#ifndef POLY_AUDIO_BUFFER_H
#define POLY_AUDIO_BUFFER_H

#include <poly/Core/Stream.h>

#include <SFML/Audio/SoundBuffer.hpp>

namespace poly
{


class Sound;


class AudioBuffer : public ReadStream
{
	friend Sound;

public:
	bool load(const std::string& fname);

	Uint32 read(void* buffer, Uint32 max) override;

	Uint32 read(Int16* samples, Uint32 max);

	bool setSamples(Int16* samples, Uint32 num, Uint32 numChannels = 1, Uint32 sampleRate = 44100);

	Uint32 getNumSamples() const;

	Uint32 getNumChannels() const;

	Uint32 getSampleRate() const;

private:
	sf::SoundBuffer m_buffer;
};


}

#endif