#ifndef POLY_MUSIC_H
#define POLY_MUSIC_H

#include <poly/Audio/AudioStream.h>

#include <poly/Core/Time.h>

#include <SFML/Audio/InputSoundFile.hpp>

namespace poly
{


///////////////////////////////////////////////////////////
class Music : public AudioStream
{
public:
	Music();

	~Music();

	bool open(const std::string& fname);

	Uint32 read(void* read, Uint32 max) override;

	void setPlayingOffset(Time offset);

	void setLoop(bool loop);

	Time getDuration() const;

	Time getPlayingOffset() const;

	bool getLoop() const;

private:
	void onSeek(Time offset) override;

private:
	sf::InputSoundFile m_file;
};


}

#endif