#pragma once
#include "MovingEntity.h"
#include "SheepStates.h"
#include "StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;



class Sheep : public MovingEntity
{
	//an instance of the state machine class
	StateMachine<Sheep>* m_pStateMachine;
	
	const int MaxSatiety = 5;

	float m_fDecreaseSatietyTime;
	float m_fIncreaseSatietyTime;
	float m_fEscapeTime;
	int	m_satiety;

	bool m_isInHouse;

	cocos2d::Animation* m_animation;
	cocos2d::Animate* m_animate;
	cocos2d::RepeatForever* m_repeatAnimate;

	virtual void SetLabel();

public:

	CREATE_FUNC(Sheep);

	virtual bool init();

	virtual bool init(cocos2d::Vec2 pos, int id);

	virtual void Update(double deltatime);

	virtual bool HandleMessage(const Telegram& msg);


	StateMachine<Sheep>* GetFSM()const { return m_pStateMachine; }

	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }

	void          ChangeLocation(location_type loc) { m_Location = loc; }

	bool          ishungry()const;
	bool          isFull()const;
	bool          isMoving()const;
	void          DecreaseSatiety();
	void          IncreaseSatiety();
	void		  SetInHouse();
	void		  SetOutHouse();

	bool		  IsOutHouse();
	bool		  IsContainedHouse();
	bool		  WantEscape();

	void PlayAnimation();
	void StopAnimation();

	void SetSpriteSpeak();
	void SetSpriteNotSpeak();

	void WanderInFence();
	void Arrive(cocos2d::Vec2 dest);
	void ArriveWithPath(std::list<cocos2d::Vec2> path, bool loop = false);
	void OffsetPursuit(MovingEntity* leader, cocos2d::Vec2 offest);

	void MoveEnd();

	Sheep();
	~Sheep();
};

