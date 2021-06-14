#ifndef POLY_AUDIO_FILE_WRITER_H
#define POLY_AUDIO_FILE_WRITER_H

#include <poly/Core/Stream.h>

#include <SFML/Audio/OutputSoundFile.hpp>

#include <mutex>

namespace poly
{


class AudioFileWriter : public WriteStream
{
public:
	AudioFileWriter();

	bool open(const std::string& fname, Uint32 sampleRate, Uint32 numChannels);

	void close();

	Uint32 write(void* data, Uint32 size) override;

	void write(Int16* samples, Uint32 num);

	void save(ReadStream* input);

	void save();

	bool isOpen() const;

private:
	sf::OutputSoundFile m_file;
	Uint32 m_sampleRate;
	Uint32 m_numChannels;
	std::mutex m_mutex;
	bool m_isOpen;
};


}

#endif