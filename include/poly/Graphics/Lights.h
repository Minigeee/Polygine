#ifndef POLY_LIGHTS_H
#define POLY_LIGHTS_H

#include <poly/Math/Vector3.h>

namespace poly
{

class Shader;


class Light
{
public:
	virtual void apply(Shader* shader, Uint32 id) = 0;

	Vector3f m_diffuse;
	Vector3f m_specular;
};


class DirectionLight : public Light
{
public:
	DirectionLight();
	DirectionLight(const Vector3f& d, const Vector3f& s, const Vector3f& dir);

	void apply(Shader* shader, Uint32 id = 0);

	Vector3f m_direction;
};

}

#endif