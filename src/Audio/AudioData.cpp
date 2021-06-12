#include <poly/Audio/AudioData.h>

namespace poly
{


///////////////////////////////////////////////////////////
AudioData::AudioData() :
	m_numChannels		(1),
	m_sampleRate		(44100)
{

}


///////////////////////////////////////////////////////////
AudioData::~AudioData()
{

}


///////////////////////////////////////////////////////////
Uint32 AudioData::getNumChannels() const
{
	return m_numChannels;
}


///////////////////////////////////////////////////////////
Uint32 AudioData::getSampleRate() const
{
	return m_sampleRate;
}


}