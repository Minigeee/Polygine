#ifndef POLY_SKELETON_H
#define POLY_SKELETON_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Bone.h>
#include <poly/Graphics/Shader.h>

namespace poly
{

class Animation;
class Bone;

class Skeleton
{
public:
	Skeleton();

	bool load(const std::string& fname);

	void apply(Shader* shader);

	void update(float dt);

	Bone* createBone(const std::string& name);

	void removeBone(const std::string& name);

	void setRoot(Bone* bone);

	void setAnimation(Animation* animation);

	void setAnimationTime(float time);

	void setAnimationSpeed(float speed);

	Bone* getBone(const std::string& name) const;

	Bone* getRoot() const;

	Uint32 getNumBones() const;

	Animation* getAnimation() const;

	float getAnimationTime() const;

	float getAnimationSpeed() const;
	
private:
	Bone* m_root;
	ObjectPool m_bonePool;
	HashMap<std::string, Bone*> m_boneMap;

	Animation* m_animation;
	float m_animTime;
	float m_animSpeed;
};

}

#endif