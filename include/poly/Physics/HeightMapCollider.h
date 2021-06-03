#ifndef POLY_HEIGHT_MAP_COLLIDER_H
#define POLY_HEIGHT_MAP_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


class Image;


class HeightMapCollider : public Collider
{
public:
	HeightMapCollider();

	HeightMapCollider(const Image& hmap, const Vector3f& dims);

	void setHeightMap(const Image& hmap);

	void setHeightData(float* data);

	void setMapSize(const Vector2u& size);

	void setMapSize(Uint32 w, Uint32 h);

	void setDimensions(const Vector3f& dims);

	void setDimensions(float x, float y, float z);

	const Vector2u& getMapSize() const;

	float* getHeightData() const;

	const Vector3f& getDimensions() const;

private:
	Vector2u m_mapSize;
	float* m_data;
	Vector3f m_dimensions;
};


}

#endif