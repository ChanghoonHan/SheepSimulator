#include "MovingEntity.h"
#include "GameWorld.h"

#include <math.h>


int Sign(const cocos2d::Vec2& v1, const cocos2d::Vec2& v2)
{
	if (v1.y*v2.x > v1.x*v2.y)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

void WrapAround(cocos2d::Vec2 &pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) { pos.x = 0.0; }

	if (pos.x < 0) { pos.x = (double)MaxX; }

	if (pos.y > MaxY) { pos.y = (double)0.0; }

	if (pos.y < 0) { pos.y = (double)MaxY; }
}

void MovingEntity::Update(double time_elapsed)
{
	//update the time elapsed
	m_dTimeElapsed = time_elapsed;

	//keep a record of its old position so we can update its cell later
	//in this method
	cocos2d::Vec2 OldPos = Pos();

	cocos2d::Vec2 SteeringForce = cocos2d::Vec2::ZERO;

	//calculate the combined force from each steering behavior in the 
	//vehicle's list
	SteeringForce = m_pSteering->Calculate();

	//Acceleration = Force/Mass
	cocos2d::Vec2 acceleration = SteeringForce / m_dMass;

	//update velocity
	m_vVelocity += acceleration * time_elapsed;

	//make sure vehicle does not exceed maximum velocity
	if (m_vVelocity.length() > m_dMaxSpeed)
	{
		m_vVelocity.normalize();

		m_vVelocity *= m_dMaxSpeed;
	}

	//update the position
	if (!m_pSteering->isStopOn())
	{
		m_pos += m_vVelocity * time_elapsed;
	
		//update the heading if the vehicle has a non zero velocity
		if (m_vVelocity.lengthSquared() > 0.00000001)
		{
			int sign = Sign(m_vVelocity.getNormalized(), m_vHeading);
			float angle = m_vVelocity.getNormalized().dot(m_vHeading);
			if (angle > 1)
			{
				angle = 1;
			}

			float temp = MATH_RAD_TO_DEG(acos(angle)) * sign;

			m_draw->clear();
			m_draw->drawLine(m_sprite->getPosition(), m_vHeading * 100, cocos2d::Color4F::BLUE);
			m_draw->drawLine(m_sprite->getPosition(), m_vVelocity.getNormalized() * 100, cocos2d::Color4F::RED);

			m_rotation += temp;
			m_sprite->setRotationSkewY(m_rotation);
			/*cocos2d::RotateBy* rotate = cocos2d::RotateBy::create(0, temp);
			m_sprite->runAction(rotate);*/

			m_vHeading = m_vVelocity.getNormalized();

			m_vSide = m_vHeading.getPerp();
		}

		//treat the screen as a toroid
		WrapAround(m_pos, GameWorld::GetInstance()->cxClient(), GameWorld::GetInstance()->cyClient());
	}
	else
	{
		m_sprite->setRotationSkewY(180);
	}
}

bool MovingEntity::init(cocos2d::Vec2 pos, int id)
{
	m_pSteering = new SteeringBehavior(this);

	m_draw = cocos2d::DrawNode::create();
	this->addChild(m_draw);

	m_rotation = 0;
	m_vHeading = cocos2d::Vec2(-1, 0);

	m_vVelocity = cocos2d::Vec2(0, 0);
	m_dMass = 1.0;
	m_vSide = m_vHeading.getPerp();
	
	if (id == 5 || id == 6)
	{
		m_dMaxSpeed = 200;
	}
	else
	{
		m_dMaxSpeed = 100;
	}
	
	m_dMaxTurnRate = 360;
	m_dMaxForce = 8000;
	m_pos = pos;
	m_id = id;
	SetPos(m_pos);
	return true;
}
