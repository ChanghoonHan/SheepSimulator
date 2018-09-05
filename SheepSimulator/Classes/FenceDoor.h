#pragma once
#include "BaseGameEntity.h"
#include "Locations.h"
#include "StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

class FenceDoor : public BaseGameEntity
{
	//an instance of the state machine class
	StateMachine<FenceDoor>* m_pStateMachine;

	location_type m_destinationLocation;

	bool m_isEndMove;

	virtual void SetLabel();

	void MoveEndCallBackFunc();
public:
	CREATE_FUNC(FenceDoor);

	bool init();
	virtual bool init(cocos2d::Vec2 pos, int id);

	virtual void Update(double deltatime);

	virtual bool HandleMessage(const Telegram& msg);

	//this must be implemented
	void Update();

	StateMachine<FenceDoor>* GetFSM()const { return m_pStateMachine; }

	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }

	void          ChangeLocation(location_type loc) { m_Location = loc; }
	void		  SetDestinationLocation(location_type loc) { m_destinationLocation = loc; }
	location_type GetDestinationLocation() { return m_destinationLocation; }

	bool          isMoving()const;

	//3¿Â ¥Î√º
	void Open();
	void Close();

	FenceDoor();
	~FenceDoor();
};

