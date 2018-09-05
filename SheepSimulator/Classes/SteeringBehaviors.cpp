#include "SteeringBehaviors.h"
#include "MovingEntity.h"
#include "GameWorld.h"
#include "Wall2D.h"
#include "Common\2D\Transformations.h"
#include "Common\2D\geometry.h"


using std::string;
using std::vector;

const double  MaxDouble = (std::numeric_limits<double>::max)();
//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SteeringBehavior::SteeringBehavior(MovingEntity* agent) :
	m_pVehicle(agent),
	m_iFlags(0),
	m_dWeightSeparation(10.0),
	m_dWeightWander(1000),
	m_dWeightWallAvoidance(500),
	m_dViewDistance(50.0),
	m_dWallDetectionFeelerLength(30),
	m_Feelers(3),
	m_Deceleration(slow),
	m_pTargetAgent1(NULL),
	m_pTargetAgent2(NULL),
	m_dWanderDistance(WanderDist),
	m_dWanderJitter(WanderJitterPerSec),
	m_dWanderRadius(WanderRad),
	m_dWaypointSeekDistSq(WaypointSeekDist*WaypointSeekDist),
	m_dWeightSeek(1.0),
	m_dWeightArrive(1.0),
	m_dWeightOffsetPursuit(1.0),
	m_dWeightFollowPath(0.05),
	m_dWeightEvade(0.01),
	m_SummingMethod(prioritized)
{
	//stuff for the wander behavior
	double theta = cocos2d::rand_0_1() * 360;

	//create a vector to a target position on the wander circle
	m_vWanderTarget = Vec2(m_dWanderRadius * cos(theta),
		m_dWanderRadius * sin(theta));
}

//---------------------------------dtor ----------------------------------
SteeringBehavior::~SteeringBehavior() {}

/////////////////////////////////////////////////////////////////////////////// CALCULATE METHODS 


//----------------------- Calculate --------------------------------------
//
//  calculates the accumulated steering force according to the method set
//  in m_SummingMethod
//------------------------------------------------------------------------
Vec2 SteeringBehavior::Calculate()
{
	//reset the steering force
	m_vSteeringForce = Vec2::ZERO;

	m_vSteeringForce = CalculatePrioritized();

	return m_vSteeringForce;
}

//------------------------- ForwardComponent -----------------------------
//
//  returns the forward oomponent of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::ForwardComponent()
{
	return m_pVehicle->Heading().dot(m_vSteeringForce);
}

//--------------------------- SideComponent ------------------------------
//  returns the side component of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::SideComponent()
{
	return m_pVehicle->Side().dot(m_vSteeringForce);
}


//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehavior::AccumulateForce(Vec2 &RunningTot, Vec2 ForceToAdd)
{

	//calculate how much steering force the vehicle has used so far
	double MagnitudeSoFar = RunningTot.length();

	//calculate how much steering force remains to be used by this vehicle
	double MagnitudeRemaining = m_pVehicle->MaxForce() - MagnitudeSoFar;

	//return false if there is no more force left to use
	if (MagnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	double MagnitudeToAdd = ForceToAdd.length();

	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this vehicle, just
	//add together. Otherwise add as much of the ForceToAdd vector is
	//possible without going over the max.
	if (MagnitudeToAdd < MagnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}

	else
	{
		//add it to the steering force
		ForceToAdd.normalize();
		RunningTot += (ForceToAdd * MagnitudeRemaining);
	}

	return true;
}



//---------------------- CalculatePrioritized ----------------------------
//
//  this method calls each active steering behavior in order of priority
//  and acumulates their forces until the max steering force magnitude
//  is reached, at which time the function returns the steering force 
//  accumulated to that  point
//------------------------------------------------------------------------
Vec2 SteeringBehavior::CalculatePrioritized()
{
	Vec2 force;

	if (On(wall_avoidance))
	{
		force = WallAvoidance(GameWorld::GetInstance()->Walls()) *
			m_dWeightWallAvoidance;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	////these next three can be combined for flocking behavior (wander is
	////also a good behavior to add into this mix)

	if (On(separation))
	{
		force = Separation(GameWorld::GetInstance()->Agents()) * m_dWeightSeparation;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(arrive))
	{
		force = Arrive(m_arrivePos, m_Deceleration) * m_dWeightArrive;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if(On(wander))
	{
		force = Wander() * m_dWeightWander;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(offset_pursuit))
	{
		force = OffsetPursuit(m_pTargetAgent1, m_vOffset);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(follow_path))
	{
		force = FollowPath() * m_dWeightFollowPath;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	return m_vSteeringForce;
}

/////////////////////////////////////////////////////////////////////////////// START OF BEHAVIORS

//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vec2 SteeringBehavior::Seek(Vec2 TargetPos)
{
	Vec2 DesiredVelocity = Vec2(TargetPos - m_pVehicle->Pos())
		* m_pVehicle->MaxSpeed();

	return (DesiredVelocity - m_pVehicle->Velocity());
}

//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
Vec2 SteeringBehavior::Arrive(Vec2     TargetPos,
	Deceleration deceleration)
{
	Vec2 ToTarget = TargetPos - m_pVehicle->Pos();

	//calculate the distance to the target
	double dist = ToTarget.length();

	if (dist > 0)
	{
		if (dist < 30)
		{
			m_pVehicle->SetIsNotMove();
			if (!isOffsetPursuitOn())
			{
				StopOn();
			}
		}

		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const double DecelerationTweaker = 0.03;

		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed = dist / ((double)deceleration * DecelerationTweaker);

		//make sure the velocity does not exceed the max
		speed = MIN(speed, m_pVehicle->MaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vec2 DesiredVelocity = ToTarget * speed / dist;

		return (DesiredVelocity - m_pVehicle->Velocity());
	}

	return Vec2(0, 0);
}

//--------------------------- Wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vec2 SteeringBehavior::Wander()
{
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	double JitterThisTimeSlice = m_dWanderJitter * GameWorld::GetInstance()->GetElapsedTime();

	//first, add a small random vector to the target's position
	m_vWanderTarget += Vec2((rand_0_1() - rand_0_1()) * JitterThisTimeSlice,
		(rand_0_1() - rand_0_1()) * JitterThisTimeSlice);

	//reproject this new vector back on to a unit circle
	m_vWanderTarget.normalize();

	//increase the length of the vector to the same as the radius
	//of the wander circle
	m_vWanderTarget *= m_dWanderRadius;

	//move the target into a position WanderDist in front of the agent
	Vec2 target = m_vWanderTarget + Vec2(m_dWanderDistance, 0);

	//project the target into world space
	Vec2 Target = PointToWorldSpace(target,
		m_pVehicle->Heading(),
		m_pVehicle->Side(),
		m_pVehicle->Pos());

	//and steer towards it
	return Target - m_pVehicle->Pos();
}

//--------------------------- WallAvoidance --------------------------------
//
//  This returns a steering force that will keep the agent away from any
//  walls it may encounter
//------------------------------------------------------------------------
Vec2 SteeringBehavior::WallAvoidance(const std::vector<Wall2D>& walls)
{
	//the feelers are contained in a std::vector, m_Feelers
	CreateFeelers();

	double DistToThisIP = 0.0;
	double DistToClosestIP = MaxDouble;

	//this will hold an index into the vector of walls
	int ClosestWall = -1;

	Vec2 SteeringForce,
		point,         //used for storing temporary info
		ClosestPoint;  //holds the closest intersection point

					   //examine each feeler in turn
	for (unsigned int flr = 0; flr<m_Feelers.size(); ++flr)
	{
		//run through each wall checking for any intersection points
		for (unsigned int w = 0; w<walls.size(); ++w)
		{
			if (LineIntersection2D(m_pVehicle->Pos(),
				m_Feelers[flr],
				walls[w].From(),
				walls[w].To(),
				DistToThisIP,
				point))
			{
				//is this the closest found so far? If so keep a record
				if (DistToThisIP < DistToClosestIP)
				{
					DistToClosestIP = DistToThisIP;

					ClosestWall = w;

					ClosestPoint = point;
				}
			}
		}//next wall


		 //if an intersection point has been detected, calculate a force  
		 //that will direct the agent away
		if (ClosestWall >= 0)
		{
			//calculate by what distance the projected position of the agent
			//will overshoot the wall
			Vec2 OverShoot = m_Feelers[flr] - ClosestPoint;

			//create a force in the direction of the wall normal, with a 
			//magnitude of the overshoot
			SteeringForce = walls[ClosestWall].Normal() * OverShoot.length();
		}

	}//next feeler

	return SteeringForce;
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void SteeringBehavior::CreateFeelers()
{
	//feeler pointing straight in front
	m_Feelers[0] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength * m_pVehicle->Heading();

	//feeler to left
	Vec2 temp = m_pVehicle->Heading();
	Vec2DRotateAroundOrigin(temp, 90 * 3.5f);
	m_Feelers[1] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength / 2.0f * temp;

	//feeler to right
	temp = m_pVehicle->Heading();
	Vec2DRotateAroundOrigin(temp, 90 * 0.5f);
	m_Feelers[2] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength / 2.0f * temp;
}


//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
Vec2 SteeringBehavior::Separation(const vector<MovingEntity*> &neighbors)
{
	Vec2 SteeringForce;

	for (unsigned int a = 0; a<neighbors.size(); ++a)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough. ***also make sure it doesn't
		//include the evade target ***
		if ((neighbors[a] != m_pVehicle) && (neighbors[a] != m_pTargetAgent1))
		{
			Vec2 ToAgent = m_pVehicle->Pos() - neighbors[a]->Pos();

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			ToAgent.normalize();
			SteeringForce += ToAgent / ToAgent.length();
		}
	}

	return SteeringForce;
}

//------------------------- Offset Pursuit -------------------------------
//
//  Produces a steering force that keeps a vehicle at a specified offset
//  from a leader vehicle
//------------------------------------------------------------------------
Vec2 SteeringBehavior::OffsetPursuit(const MovingEntity*  leader, const Vec2 offset)
{
	//calculate the offset's position in world space
	Vec2 WorldOffsetPos = PointToWorldSpace(offset,
		leader->Heading(),
		leader->Side(),
		leader->Pos());

	Vec2 ToOffset = WorldOffsetPos - m_pVehicle->Pos();

	//the lookahead time is propotional to the distance between the leader
	//and the pursuer; and is inversely proportional to the sum of both
	//agent's velocities
	double LookAheadTime = ToOffset.length() /
		(m_pVehicle->MaxSpeed() + leader->Speed());

	//now Arrive at the predicted future position of the offset
	return Arrive(WorldOffsetPos + leader->Velocity() * LookAheadTime, fast);
}

//------------------------------- FollowPath -----------------------------
//
//  Given a series of Vector2Ds, this method produces a force that will
//  move the agent along the waypoints in order. The agent uses the
// 'Seek' behavior to move to the next waypoint - unless it is the last
//  waypoint, in which case it 'Arrives'
//------------------------------------------------------------------------
Vec2 SteeringBehavior::FollowPath()
{
	//move to next target if close enough to current target (working in
	//distance squared space)
	if (m_pPath->CurrentWaypoint().getDistanceSq(m_pVehicle->Pos()) < m_dWaypointSeekDistSq)
	{
		m_pPath->SetNextWaypoint();
	}

	if (!m_pPath->Finished())
	{
		return Seek(m_pPath->CurrentWaypoint());
	}
	else
	{
		return Arrive(m_pPath->CurrentWaypoint(), normal);
	}
}

void SteeringBehavior::SetPath(Path* path)
{
	m_pPath = path;
}

