#ifndef POLY_LIGHTING_H
#define POLY_LIGHTING_H

#include <poly/Engine/Extension.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class Shader;

class Lighting : public Extension
{
public:
	Lighting(Scene* scene);

	void apply(Shader* shader);

	void setAmbientColor(const Vector3f& color);

	void setAmbientColor(float r, float g, float b);

	const Vector3f& getAmbientColor() const;

private:
	Vector3f m_ambientColor;
};

}

#endif