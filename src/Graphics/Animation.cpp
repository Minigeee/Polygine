#include <poly/Core/Logger.h>

#include <poly/Graphics/Animation.h>

#include <poly/Math/Transform.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <set>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
Matrix4f aiMatrixToMatrix4f(const aiMatrix4x4& t)
{
#ifdef USE_COLUMN_MAJOR
	return Matrix4f(
		t.a1, t.b1, t.c1, t.d1,
		t.a2, t.b2, t.c2, t.d2,
		t.a3, t.b3, t.c3, t.d3,
		t.a4, t.b4, t.c4, t.d4
	);
#else
	return Matrix4f(
		t.a1, t.a2, t.a3, t.a4,
		t.b1, t.b2, t.b3, t.b4,
		t.c1, t.c2, t.c3, t.c4,
		t.d1, t.d2, t.d3, t.d4
	);
#endif
}


///////////////////////////////////////////////////////////
void getBoneNames(aiNode* node, const aiScene* scene, HashMap<std::string, Matrix4f>& transforms)
{
	// For each mesh in the node, add the list of bones
	for (Uint32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		for (Uint32 b = 0; b < mesh->mNumBones; ++b)
		{
			// Add bone name
			aiBone* bone = mesh->mBones[b];
			transforms[bone->mName.C_Str()] = Matrix4f(1.0f);
		}
	}

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		getBoneNames(node->mChildren[i], scene, transforms);
}


///////////////////////////////////////////////////////////
void getBoneTransforms(aiNode* node, const aiScene* scene, HashMap<std::string, Matrix4f>& transforms)
{
	// If the node name matches a bone name, set the id
	auto it = transforms.find(node->mName.C_Str());
	if (it != transforms.end())
		it.value() = aiMatrixToMatrix4f(node->mTransformation);

	// Process all children nodes
	for (Uint32 i = 0; i < node->mNumChildren; ++i)
		getBoneTransforms(node->mChildren[i], scene, transforms);
}


}


///////////////////////////////////////////////////////////
Animation::Animation() :
	m_duration			(0.0f),
	m_ticksPerSecond	(0.0f)
{

}


///////////////////////////////////////////////////////////
Animation::Animation(const std::string& fname, const std::string& name) :
	m_duration			(0.0f),
	m_ticksPerSecond	(0.0f)
{
	load(fname, name);
}


///////////////////////////////////////////////////////////
bool Animation::load(const std::string& fname, const std::string& name)
{
	// Load the scene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fname, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_WARNING("Failed to load skeleton file %s: %s", fname.c_str(), importer.GetErrorString());
		return false;
	}

	// Find correct animation
	aiAnimation* anim = 0;
	for (Uint32 i = 0; i < scene->mNumAnimations; ++i)
	{
		if (strcmp(scene->mAnimations[i]->mName.C_Str(), name.c_str()) == 0)
		{
			anim = scene->mAnimations[i];
			break;
		}
	}

	// Quit if animation was not found
	if (!anim)
	{
		LOG_WARNING("Animation \"%s\" was not found in file %s", name.c_str(), fname.c_str());
		return false;
	}

	// Find all bones and their default transforms
	priv::getBoneNames(scene->mRootNode, scene, m_bindPose);
	priv::getBoneTransforms(scene->mRootNode, scene, m_bindPose);

	// Create channels
	for (Uint32 c = 0; c < anim->mNumChannels; ++c)
	{
		aiNodeAnim* aiChannel = anim->mChannels[c];

		Channel& channel = m_channels[aiChannel->mNodeName.C_Str()];

		aiVectorKey* p = aiChannel->mPositionKeys;
		aiQuatKey* r = aiChannel->mRotationKeys;
		aiVectorKey* s = aiChannel->mScalingKeys;

		// Reserve first spot for first frame
		channel.m_times.push_back(0.0f);
		channel.m_positions.push_back(Vector3f());
		channel.m_rotations.push_back(Quaternion());
		channel.m_scales.push_back(Vector3f());

		// Get frame times
		std::set<float> times;
		for (Uint32 i = 0; i < aiChannel->mNumPositionKeys; ++i)
			times.insert((float)aiChannel->mPositionKeys[i].mTime);
		for (Uint32 i = 0; i < aiChannel->mNumRotationKeys; ++i)
			times.insert((float)aiChannel->mRotationKeys[i].mTime);
		for (Uint32 i = 0; i < aiChannel->mNumScalingKeys; ++i)
			times.insert((float)aiChannel->mScalingKeys[i].mTime);

		// Add frames to channel, with interpolation if frames don't match up
		for (auto it = times.begin(); it != times.end(); ++it)
		{
			float time = *it;

			// Get interpolated position
			Uint32 index = 1;
			for (; index < aiChannel->mNumPositionKeys && p[index].mTime < time; ++index);
			--index;

			float factor = (float)((time - p[index].mTime) / (p[index + 1].mTime - p[index].mTime));
			aiVector3D pos = p[index].mValue + (p[index + 1].mValue - p[index].mValue) * factor;

			// Get interpolated rotation
			index = 1;
			for (; index < aiChannel->mNumRotationKeys && r[index].mTime < time; ++index);
			--index;

			factor = (float)((time - r[index].mTime) / (r[index + 1].mTime - r[index].mTime));
			Quaternion q1(r[index].mValue.x, r[index].mValue.y, r[index].mValue.z, r[index].mValue.w);
			Quaternion q2(r[index + 1].mValue.x, r[index + 1].mValue.y, r[index + 1].mValue.z, r[index + 1].mValue.w);
			q1 = normalize(q1);
			q2 = normalize(q2);

			// If the path from rot1 to rot2 is too long, take another path
			Quaternion qd = inverse(q1) * q2;
			float diff = 2.0f * atan2(length(Vector3f(qd.x, qd.y, qd.z)), qd.w);
			if (fabsf(diff) > 3.141592f)
				q2 = -q2;

			Quaternion rot = slerp(q1, q2, factor);

			// Get interpolated scale
			index = 1;
			for (; index < aiChannel->mNumScalingKeys && s[index].mTime < time; ++index);
			--index;

			factor = (float)((time - s[index].mTime) / (s[index + 1].mTime - s[index].mTime));
			aiVector3D scale = s[index].mValue + (s[index + 1].mValue - s[index].mValue) * factor;

			// Add frame to channel
			channel.m_times.push_back(time);
			channel.m_positions.push_back(Vector3f(pos.x, pos.y, pos.z));
			channel.m_rotations.push_back(rot);
			channel.m_scales.push_back(Vector3f(scale.x, scale.y, scale.z));
		}

		channel.m_positions[0] = channel.m_positions.back();
		channel.m_rotations[0] = channel.m_rotations.back();
		channel.m_scales[0] = channel.m_scales.back();

		// Fix all quaternion rotations
		for (Uint32 i = 0; i < channel.m_rotations.size() - 1; ++i)
		{
			Quaternion& q1 = channel.m_rotations[i];
			Quaternion& q2 = channel.m_rotations[i + 1];
			Quaternion qd = inverse(q1) * q2;
			float diff = 2.0f * atan2(length(Vector3f(qd.x, qd.y, qd.z)), qd.w);

			// Invert quat if the angle difference is too much
			if (fabsf(diff) > 3.141592f)
				q2 = -q2;
		}
	}

	m_name = name;
	m_duration = (float)anim->mDuration;
	m_ticksPerSecond = (float)anim->mTicksPerSecond;

	return true;
}


///////////////////////////////////////////////////////////
void Animation::addChannel(const std::string& bone, const Channel& channel)
{
	m_channels[bone] = channel;
}


///////////////////////////////////////////////////////////
void Animation::removeChannel(const std::string& bone)
{
	auto it = m_channels.find(bone);
	if (it != m_channels.end())
		m_channels.erase(it);
}


///////////////////////////////////////////////////////////
void Animation::setName(const std::string& name)
{
	m_name = name;
}


///////////////////////////////////////////////////////////
void Animation::setDuration(float duration)
{
	m_duration = duration;
}


///////////////////////////////////////////////////////////
void Animation::setTicksPerSecond(float tps)
{
	m_ticksPerSecond = tps;
}


///////////////////////////////////////////////////////////
const std::string& Animation::getName() const
{
	return m_name;
}


///////////////////////////////////////////////////////////
Matrix4f Animation::getTransform(const std::string& bone, float time) const
{
	// Find the channel
	auto it = m_channels.find(bone);
	if (it == m_channels.end())
	{
		auto matIt = m_bindPose.find(bone);
		if (matIt != m_bindPose.end())
			return matIt.value();
		return Matrix4f(1.0f);
	}

	const Channel& channel = it.value();

	// Adjust time from seconds to ticks
	time = fmod(time * m_ticksPerSecond, m_duration);

	// Find frame index
	Uint32 index = 1;
	for (; index < channel.m_times.size() && channel.m_times[index] < time; ++index);
	--index;

	// Calculate interpolation factor
	float factor = (time - channel.m_times[index]) / (channel.m_times[index + 1] - channel.m_times[index]);

	// Calculated interpolated transforms
	Vector3f position = channel.m_positions[index] + (channel.m_positions[index + 1] - channel.m_positions[index]) * factor;
	Vector3f scale = channel.m_scales[index] + (channel.m_scales[index + 1] - channel.m_scales[index]) * factor;
	Quaternion rotation = slerp(channel.m_rotations[index], channel.m_rotations[index + 1], factor);

	// Return final matrix form
	return toTransformMatrix(position, rotation, scale);
}


///////////////////////////////////////////////////////////
float Animation::getDuration(bool inSeconds) const
{
	if (inSeconds)
		return m_duration / m_ticksPerSecond;
	else
		return m_duration;
}


///////////////////////////////////////////////////////////
float Animation::getTicksPerSecond() const
{
	return m_ticksPerSecond;
}


}