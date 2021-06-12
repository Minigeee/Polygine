#ifndef POLY_AUDIO_BUFFER_H
#define POLY_AUDIO_BUFFER_H

#include <poly/Audio/AudioData.h>

#include <SFML/Audio/SoundBuffer.hpp>

namespace poly
{


class Sound;


class AudioBuffer : public AudioData
{
	friend Sound;

public:
	bool load(const std::string& fname);

	Uint32 read(Int16* samples, Uint32 max) override;

	bool setSamples(Int16* samples, Uint32 num, Uint32 numChannels = 1, Uint32 sampleRate = 44100);

	Uint32 getNumSamples() const;

private:
	sf::SoundBuffer m_buffer;
};


}

#endif