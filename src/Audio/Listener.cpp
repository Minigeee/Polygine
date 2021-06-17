#include <poly/Audio/Listener.h>

#include <SFML/Audio/Listener.hpp>

namespace poly
{


///////////////////////////////////////////////////////////
void Listener::setGlobalVolume(float volume)
{
	sf::Listener::setGlobalVolume(volume);
}


///////////////////////////////////////////////////////////
void Listener::setPosition(const Vector3f& pos)
{
	sf::Listener::setPosition(pos.x, pos.y, pos.z);
}


///////////////////////////////////////////////////////////
void Listener::setPosition(float x, float y, float z)
{
	sf::Listener::setPosition(x, y, z);
}


///////////////////////////////////////////////////////////
void Listener::setDirection(const Vector3f& dir)
{
	sf::Listener::setDirection(dir.x, dir.y, dir.z);
}


///////////////////////////////////////////////////////////
void Listener::setDirection(float x, float y, float z)
{
	sf::Listener::setDirection(x, y, z);
}


///////////////////////////////////////////////////////////
void Listener::setUpVector(const Vector3f& up)
{
	sf::Listener::setUpVector(up.x, up.y, up.z);
}


///////////////////////////////////////////////////////////
void Listener::setUpVector(float x, float y, float z)
{
	sf::Listener::setUpVector(x, y, z);
}


}