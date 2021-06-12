#ifndef POLY_AUDIO_COMPONENTS_H
#define POLY_AUDIO_COMPONENTS_H

namespace poly
{


class AudioSource;


///////////////////////////////////////////////////////////
enum class AudioStatus
{
	Play,
	Pause,
	Stop
};


///////////////////////////////////////////////////////////
struct AudioComponent
{
	AudioComponent();

	AudioSource* m_source;
	AudioStatus m_status;
	float m_pitch;
	float m_volume;
	float m_minDistance;
	float m_attenuation;
};


}

#endif