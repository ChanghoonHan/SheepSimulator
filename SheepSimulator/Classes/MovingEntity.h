#pragma once
#ifndef MOVING_ENTITY
#define MOVING_ENTITY
//------------------------------------------------------------------------
//
//  Name:   MovingEntity.h
//
//  Desc:   A base class defining an entity that moves. The entity has 
//          a local coordinate system and members for defining its
//          mass and velocity.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include "BaseGameEntity.h"
#include "SteeringBehaviors.h"

class MovingEntity : public BaseGameEntity
{
protected:

	Path m_path;

	cocos2d::Vec2    m_vVelocity;

	double m_rotation;

	//a normalized vector pointing in the direction the entity is heading. 
	cocos2d::Vec2    m_vHeading;

	//a vector perpendicular to the heading vector
	cocos2d::Vec2    m_vSide;

	double       m_dMass;

	//the maximum speed this entity may travel at.
	double       m_dMaxSpeed;

	//the maximum force this entity can produce to power itself 
	//(think rockets and thrust)
	double        m_dMaxForce;

	//the maximum rate (radians per second)this vehicle can rotate         
	double       m_dMaxTurnRate;

	double		 m_dTimeElapsed;

	bool         m_isEndMove;

	//the steering behavior class
	SteeringBehavior*     m_pSteering;

	cocos2d::DrawNode* m_draw;

public:

	MovingEntity(){}
	~MovingEntity() {}

	virtual bool init() = 0;

	virtual bool init(cocos2d::Vec2 pos, int id);

	virtual void Update(double time_elapsed);

	virtual bool HandleMessage(const Telegram& msg) = 0;

	//accessors
	cocos2d::Vec2  Velocity()const { return m_vVelocity; }
	void      SetVelocity(const cocos2d::Vec2& NewVel) { m_vVelocity = NewVel; }
	void	  SetIsNotMove() { m_isEndMove = true; }

	double     Mass()const { return m_dMass; }

	cocos2d::Vec2  Side()const { return m_vSide; }

	double     MaxSpeed()const { return m_dMaxSpeed; }
	void      SetMaxSpeed(double new_speed) { m_dMaxSpeed = new_speed; }

	double     MaxForce()const { return m_dMaxForce; }
	void      SetMaxForce(double mf) { m_dMaxForce = mf; }

	bool      IsSpeedMaxedOut()const { return m_dMaxSpeed*m_dMaxSpeed >= m_vVelocity.lengthSquared(); }
	double     Speed()const { return m_vVelocity.length(); }
	double     SpeedSq()const { return m_vVelocity.lengthSquared(); }

	cocos2d::Vec2  Heading()const { return m_vHeading; }

	double     MaxTurnRate()const { return m_dMaxTurnRate; }
	void      SetMaxTurnRate(double val) { m_dMaxTurnRate = val; }

};


#endif