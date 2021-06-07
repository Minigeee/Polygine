#ifndef POLY_JOINTS_H
#define POLY_JOINTS_H

#include <poly/Math/Vector2.h>

namespace poly
{


class Physics;


class Joint
{
	friend Physics;

public:
	enum Type
	{
		BallAndSocket,
		Hinge,
		Slider,
		Fixed
	};

public:
	Joint();

	void setLimits(float min, float max);

	Type getType() const;

	const Vector2f& getLimits() const;

private:
	void setJoint(void* joint, Type type);

private:
	void* m_joint;				//!< The internal joint object
	Type m_type;				//!< The type of joint
	Vector2f m_limits;			//!< The joint limits (for hinge and slider joints)
};


}

#endif