#ifndef POLY_ANIMATION_H
#define POLY_ANIMATION_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Matrix4.h>
#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

#include <assimp/scene.h>

#include <string>
#include <vector>

namespace poly
{

namespace priv
{

Matrix4f aiMatrixToMatrix4f(const aiMatrix4x4& t);

}


class Animation
{
public:
	struct Channel
	{
		std::vector<float> m_times;
		std::vector<Vector3f> m_positions;
		std::vector<Quaternion> m_rotations;
		std::vector<Vector3f> m_scales;
	};

public:
	Animation();

	bool load(const std::string& fname, const std::string& name);

	void addChannel(const std::string& bone, const Channel& channel);

	void removeChannel(const std::string& bone);

	void setName(const std::string& name);

	void setDuration(float duration);

	void setTicksPerSecond(float tps);

	const std::string& getName() const;

	Matrix4f getTransform(const std::string& bone, float time) const;

	float getDuration(bool inSeconds = false) const;

	float getTicksPerSecond() const;

private:
	std::string m_name;
	float m_duration;
	float m_ticksPerSecond;

	HashMap<std::string, Channel> m_channels;
	HashMap<std::string, Matrix4f> m_bindPose;
};

}

#endif