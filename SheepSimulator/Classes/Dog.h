#pragma once
#include "MovingEntity.h"
#include "Locations.h"
#include "StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;


class Dog : public MovingEntity
{
	//an instance of the state machine class
	StateMachine<Dog>* m_pStateMachine;

	virtual void SetLabel();

	int m_EscapeSheepID;

	const int maxTired = 50;

	float m_IncreaseTiredTimer;
	float m_DecreaseTiredTimer;

	int m_tired;

public:

	CREATE_FUNC(Dog);

	virtual bool init();
	virtual bool init(cocos2d::Vec2 pos, int id);

	virtual void Update(double deltatime);

	virtual bool HandleMessage(const Telegram& msg);

	StateMachine<Dog>* GetFSM()const { return m_pStateMachine; }

	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }

	void          ChangeLocation(location_type loc) { m_Location = loc; }

	bool          isMoving()const;

	void Arrive(cocos2d::Vec2 dest);
	void ArriveWithPath(std::list<cocos2d::Vec2> path, bool loop = false);
	void OffsetPursuit(MovingEntity* leader, cocos2d::Vec2 offest);

	void MoveEnd();

	void IncreaseTired();
	void DecreaseTired();
	inline int GetTired() { return m_tired; }

	inline void SetEscapeSheepId(int id) { m_EscapeSheepID = id; }
	inline int GetEscapeSheepID() { return m_EscapeSheepID; }

	Dog();
	~Dog();
};

