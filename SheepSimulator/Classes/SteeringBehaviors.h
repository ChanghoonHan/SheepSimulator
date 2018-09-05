#pragma once
#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   SteeringBehaviors.h
//
//  Desc:   class to encapsulate steering behaviors for a Vehicle
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <vector>
#include <string>
#include <list>
#include <cocos2d.h>
#include "Path.h"


class Wall2D;
class MovingEntity;

//--------------------------- Constants ----------------------------------

//the radius of the constraining circle for the wander behavior
const double WanderRad = 1.5;
//distance the wander circle is projected in front of the agent
const double WanderDist = 15;
//the maximum amount of displacement along the circle each frame
const double WanderJitterPerSec = 80;

//used in path following
const double WaypointSeekDist = 20;



//------------------------------------------------------------------------

class SteeringBehavior
{
public:

	enum summing_method { weighted_average, prioritized, dithered };

private:

	enum behavior_type
	{
		none = 0x00000,
		seek = 0x00002,
		flee = 0x00004, 
		arrive = 0x00008,
		wander = 0x00010,
		cohesion = 0x00020,
		separation = 0x00040,
		allignment = 0x00080,
		obstacle_avoidance = 0x00100,
		wall_avoidance = 0x00200,
		follow_path = 0x00400,
		pursuit = 0x00800,
		evade = 0x01000,
		interpose = 0x02000,
		hide = 0x04000,
		flock = 0x08000,
		offset_pursuit = 0x10000,
		stop = 0x20000,
	};

private:
	//a pointer to the owner of this instance
	MovingEntity*     m_pVehicle;

	//the steering force created by the combined effect of all
	//the selected behaviors
	cocos2d::Vec2    m_vSteeringForce;

	//these can be used to keep track of friends, pursuers, or prey
	MovingEntity*     m_pTargetAgent1;
	MovingEntity*     m_pTargetAgent2;

	//the current target
	cocos2d::Vec2    m_vTarget;

	cocos2d::Vec2    m_arrivePos;

	//a vertex buffer to contain the feelers rqd for wall avoidance  
	std::vector<cocos2d::Vec2> m_Feelers;

	//the length of the 'feeler/s' used in wall detection
	double                 m_dWallDetectionFeelerLength;

	//the current position on the wander circle the agent is
	//attempting to steer towards
	cocos2d::Vec2     m_vWanderTarget;

	//explained above
	double        m_dWanderJitter;
	double        m_dWanderRadius;
	double        m_dWanderDistance;


	//multipliers. These can be adjusted to effect strength of the  
	//appropriate behavior. Useful to get flocking the way you require
	//for example.
	double        m_dWeightSeparation;
	double        m_dWeightWander;
	double        m_dWeightWallAvoidance;
	double        m_dWeightSeek;
	double        m_dWeightArrive;
	double        m_dWeightOffsetPursuit;
	double        m_dWeightEvade;
	double		  m_dWeightFollowPath;

	//how far the agent can 'see'
	double        m_dViewDistance;

	//the distance (squared) a vehicle has to be from a path waypoint before
	//it starts seeking to the next waypoint
	double        m_dWaypointSeekDistSq;


	//any offset used for formations or offset pursuit
	cocos2d::Vec2     m_vOffset;

	Path* m_pPath;

	//binary flags to indicate whether or not a behavior should be active
	int           m_iFlags;


	//Arrive makes use of these to determine how quickly a vehicle
	//should decelerate to its target
	enum Deceleration { slow = 3, normal = 2, fast = 1 };

	//default
	Deceleration m_Deceleration;

	//is cell space partitioning to be used or not?
	bool          m_bCellSpaceOn;

	//what type of method is used to sum any active behavior
	summing_method  m_SummingMethod;


	//this function tests if a specific bit of m_iFlags is set
	bool      On(behavior_type bt) { return (m_iFlags & bt) == bt; }

	bool      AccumulateForce(cocos2d::Vec2 &sf, cocos2d::Vec2 ForceToAdd);

	//creates the antenna utilized by the wall avoidance behavior
	void      CreateFeelers();



	/* .......................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................*/
	cocos2d::Vec2 SteeringBehavior::FollowPath();

	//this behavior moves the agent towards a target position
	cocos2d::Vec2 Seek(cocos2d::Vec2 TargetPos);

	//this behavior is similar to seek but it attempts to arrive 
	//at the target position with a zero velocity
	cocos2d::Vec2 Arrive(cocos2d::Vec2     TargetPos,
		Deceleration deceleration);

	//this behavior maintains a position, in the direction of offset
	//from the target vehicle
	cocos2d::Vec2 OffsetPursuit(const MovingEntity* agent, const cocos2d::Vec2 offset);

	//this behavior makes the agent wander about randomly
	cocos2d::Vec2 Wander();

	//this returns a steering force which will keep the agent away from any
	//walls it may encounter
	cocos2d::Vec2 WallAvoidance(const std::vector<Wall2D> &walls);

	// -- Group Behaviors -- //
	cocos2d::Vec2 Separation(const std::vector<MovingEntity*> &agents);

	/* .......................................................

	END BEHAVIOR DECLARATIONS

	.......................................................*/

	cocos2d::Vec2 CalculatePrioritized();

	void SetPath(Path* path);

public:

	SteeringBehavior(MovingEntity* agent);

	virtual ~SteeringBehavior();

	//calculates and sums the steering forces from any active behaviors
	cocos2d::Vec2 Calculate();

	//calculates the component of the steering force that is parallel
	//with the vehicle heading
	double    ForwardComponent();

	//calculates the component of the steering force that is perpendicuar
	//with the vehicle heading
	double    SideComponent();

	void      SetTarget(const cocos2d::Vec2 t) { m_vTarget = t; }

	void      SetTargetAgent1(MovingEntity* Agent) { m_pTargetAgent1 = Agent; }
	void      SetTargetAgent2(MovingEntity* Agent) { m_pTargetAgent2 = Agent; }

	void      SetOffset(const cocos2d::Vec2 offset) { m_vOffset = offset; }
	cocos2d::Vec2  GetOffset()const { return m_vOffset; }

	cocos2d::Vec2 Force()const { return m_vSteeringForce; }

	void      ToggleSpacePartitioningOnOff() { m_bCellSpaceOn = !m_bCellSpaceOn; }
	bool      isSpacePartitioningOn()const { return m_bCellSpaceOn; }

	void      SetSummingMethod(summing_method sm) { m_SummingMethod = sm; }


	void ArriveOn(cocos2d::Vec2 pos) { m_arrivePos = pos; m_iFlags |= arrive; }
	void WanderOn() { m_iFlags |= wander; }
	void SeparationOn() { m_iFlags |= separation; }
	void WallAvoidanceOn() { m_iFlags |= wall_avoidance; }
	void FollowPathOn(Path* path, bool loop = false) { 
		m_pPath = path, m_iFlags |= follow_path; 
		if (loop)
		{
			m_pPath->LoopOn();
		}
		else
		{
			m_pPath->LoopOff();
		}
	}
	void InterposeOn(MovingEntity* v1, MovingEntity* v2) { m_iFlags |= interpose; m_pTargetAgent1 = v1; m_pTargetAgent2 = v2; }
	void HideOn(MovingEntity* v) { m_iFlags |= hide; m_pTargetAgent1 = v; }
	void OffsetPursuitOn(MovingEntity* v1, const cocos2d::Vec2 offset) { m_iFlags |= offset_pursuit; m_vOffset = offset; m_pTargetAgent1 = v1; }
	void ArriveOff() { if (On(arrive)) m_iFlags ^= arrive; }
	void WanderOff() { if (On(wander)) m_iFlags ^= wander; }
	void FollowPathOff() { if (On(follow_path)) m_iFlags ^= follow_path; }
	void SeparationOff() { if (On(separation)) m_iFlags ^= separation; }
	void WallAvoidanceOff() { if (On(wall_avoidance)) m_iFlags ^= wall_avoidance; }
	void OffsetPursuitOff() { if (On(offset_pursuit)) m_iFlags ^= offset_pursuit; }

	void StopOn() { m_iFlags |= stop; }
	void StopOff() { if (On(stop)) m_iFlags ^= stop; }
	bool isStopOn() { return On(stop); }

	bool isArriveOn() { return On(arrive); }
	bool isWanderOn() { return On(wander); }
	bool isSeparationOn() { return On(separation); }
	bool isWallAvoidanceOn() { return On(wall_avoidance); }
	bool isOffsetPursuitOn() { return On(offset_pursuit); }

	const std::vector<cocos2d::Vec2>& GetFeelers()const { return m_Feelers; }

	double WanderJitter()const { return m_dWanderJitter; }
	double WanderDistance()const { return m_dWanderDistance; }
	double WanderRadius()const { return m_dWanderRadius; }

	double SeparationWeight()const { return m_dWeightSeparation; }
};

#endif