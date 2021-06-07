#include <poly/Core/Logger.h>

#include <poly/Physics/Joints.h>

#include <reactphysics3d/reactphysics3d.h>

#define BNS_CAST(x) reinterpret_cast<reactphysics3d::BallAndSocketJoint*>(x)
#define HINGE_CAST(x) reinterpret_cast<reactphysics3d::HingeJoint*>(x)
#define SLIDER_CAST(x) reinterpret_cast<reactphysics3d::SliderJoint*>(x)
#define FIXED_CAST(x) reinterpret_cast<reactphysics3d::FixedJoint*>(x)

namespace poly
{


///////////////////////////////////////////////////////////
Joint::Joint() :
	m_joint					(0),
	m_type					(Type::BallAndSocket),
	m_limits				(0.0f)
{

}


///////////////////////////////////////////////////////////
void Joint::setLimits(float min, float max)
{
	ASSERT(m_type == Hinge || m_type == Slider, "Limits can only be applied to hinge and slider joints");

	if (m_type == Hinge)
	{
		reactphysics3d::HingeJoint* joint = HINGE_CAST(m_joint);
		joint->enableLimit(true);
		joint->setMinAngleLimit(min);
		joint->setMinAngleLimit(max);
	}
	else
	{
		reactphysics3d::SliderJoint* joint = SLIDER_CAST(m_joint);
		joint->enableLimit(true);
		joint->setMinTranslationLimit(min);
		joint->setMaxTranslationLimit(max);
	}

	m_limits = Vector2f(min, max);
}


///////////////////////////////////////////////////////////
Joint::Type Joint::getType() const
{
	return m_type;
}


///////////////////////////////////////////////////////////
const Vector2f& Joint::getLimits() const
{
	return m_limits;
}


///////////////////////////////////////////////////////////
void Joint::setJoint(void* joint, Joint::Type type)
{
	m_joint = joint;
	m_type = type;
}


}