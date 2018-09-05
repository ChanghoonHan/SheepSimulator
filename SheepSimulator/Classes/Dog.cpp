#include "Dog.h"
#include "DogStates.h"

USING_NS_CC;

void Dog::SetLabel()
{
	m_label->setString(StringUtils::format("Loc : %s\nTired : %d\nVoice : %s\nx : %lf\ny : %lf\nrot : %lf", GetLocationStringFromType().c_str(), m_tired, m_voice.c_str(), this->getPositionX(), this->getPositionY(), m_rotation));
}

bool Dog::init()
{
	m_isEndMove = false;
	m_visiableSize = Director::getInstance()->getVisibleSize();
	m_sprite = Sprite::create("Dog.png");
	m_sprite->setFlipX(true);
	m_label = Label::createWithSystemFont("", "Thonburi", 15);
	SetLabel();
	addChild(m_sprite);
	addChild(m_label);
	SetPos(Vec2(0, 0));
	return true;
}

bool Dog::init(Vec2 pos, int id)
{
	MovingEntity::init(pos, id);
	
	Arrive(pos);

	m_id = id;

	m_IncreaseTiredTimer = 0;
	m_DecreaseTiredTimer = 0;

	m_tired = 25;

	m_pStateMachine = new StateMachine<Dog>(this);

	m_pStateMachine->SetGlobalState(GlobalState::Instance());

	if (id == 5)
	{
		m_pStateMachine->SetCurrentState(GoFenceAndPatrol::Instance());
		m_pStateMachine->CurrentState()->Enter(this);
		m_Location = fencePatrol;
	}
	else
	{
		m_pStateMachine->SetCurrentState(GoHoomAndTakeARest::Instance());
		m_Location = dogHouse;
	}

	m_pos = pos;
	SetPos(m_pos);

	return true;
}

void Dog::Update(double deltatime)
{
	m_pos = this->getPosition();

	m_IncreaseTiredTimer += deltatime;
	m_DecreaseTiredTimer += deltatime;

	m_pStateMachine->Update();
	MovingEntity::Update(deltatime);
	SetPos(m_pos);

	SetLabel();
}

bool Dog::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

bool Dog::isMoving()const
{
	return !m_isEndMove;
}

void Dog::Arrive(cocos2d::Vec2 dest)
{
	m_pSteering->StopOff();
	m_isEndMove = false;
	m_pSteering->WallAvoidanceOff();
	m_pSteering->WanderOff();
	m_pSteering->FollowPathOff();
	m_pSteering->OffsetPursuitOff();
	m_pSteering->ArriveOn(dest);
	m_pSteering->SeparationOff();
}

void Dog::ArriveWithPath(std::list<cocos2d::Vec2> path, bool loop)
{
	m_isEndMove = false;
	m_path.Set(path);
	m_pSteering->StopOff();
	m_pSteering->FollowPathOn(&m_path, loop);
	m_pSteering->WallAvoidanceOff();
	m_pSteering->WanderOff();
	m_pSteering->ArriveOff();
	m_pSteering->OffsetPursuitOff();
	m_pSteering->SeparationOff();
}

void Dog::OffsetPursuit(MovingEntity* leader, cocos2d::Vec2 offest)
{
	m_pSteering->StopOff();
	m_pSteering->WallAvoidanceOff();
	m_pSteering->WanderOff();
	m_pSteering->ArriveOff();
	m_pSteering->FollowPathOff();
	m_pSteering->OffsetPursuitOn(leader, offest);
	m_pSteering->SeparationOn();
}

void Dog::IncreaseTired()
{
	if (m_IncreaseTiredTimer > 1)
	{
		m_IncreaseTiredTimer = 0;
		if (rand_0_1() > 0.5)
		{
			m_tired += 1;
		}
	}
}

void Dog::DecreaseTired()
{
	if (m_DecreaseTiredTimer > 1)
	{
		m_DecreaseTiredTimer = 0;
		m_tired -= 2;
		if (m_tired < 0)
		{
			m_tired = 0;
		}
	}
}


void Dog::MoveEnd()
{
	m_isEndMove = true;
}

Dog::Dog()
{
}


Dog::~Dog()
{
	delete m_pStateMachine;
}
