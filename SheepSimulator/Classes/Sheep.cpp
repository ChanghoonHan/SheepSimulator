#include "Sheep.h"
#include "EntityManager.h"

USING_NS_CC;

void Sheep::SetLabel()
{
	//m_label->setString(StringUtils::format("Loc : %s\nSat : %d\nVoice : %s", GetLocationStringFromType().c_str(), m_satiety, m_voice.c_str()));
	m_label->setString(StringUtils::format("Loc : %s\nSat : %d\nVoice : %s\nx : %lf\ny : %lf\nrot : %lf", GetLocationStringFromType().c_str(), m_satiety, m_voice.c_str(), this->getPositionX(), this->getPositionY(), m_rotation));
}

bool Sheep::init()
{
	m_isEndMove = true;
	m_fDecreaseSatietyTime = 0;
	m_fIncreaseSatietyTime = 0;
	m_fEscapeTime = 0;
	m_visiableSize = Director::getInstance()->getVisibleSize();
	m_sprite = Sprite::create("Sheep.png");
	m_label =  Label::createWithSystemFont("", "Thonburi", 15);
	SetLabel();
	addChild(m_sprite);
	addChild(m_label);
	SetPos(Vec2(m_visiableSize.width / 2, m_visiableSize.height / 2));
	m_isPlayAnimation = false;
	return true;
}

bool Sheep::init(Vec2 pos, int id)
{
	m_pStateMachine = new StateMachine<Sheep>(this);

	m_pStateMachine->SetCurrentState(EnterSheepHouseAndPlay::Instance());

	m_Location = sheepHouse;

 	m_satiety = MaxSatiety;

	MovingEntity::init(pos, id);
	if (id == 0)
	{
		m_sprite->setColor(Color3B(128, 128, 128));
	}
	
	WanderInFence();

	return true;
}

void Sheep::Update(double deltatime)
{
	m_pos = this->getPosition();
	m_fDecreaseSatietyTime += deltatime;
	m_fIncreaseSatietyTime += deltatime;
	m_fEscapeTime += deltatime;
	
	m_pStateMachine->Update();
	MovingEntity::Update(deltatime);
	SetPos(m_pos);

	SetLabel();
}

bool Sheep::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

bool Sheep::ishungry()const
{
	if (m_Location == sheepHouse && m_satiety < 3)
	{
		return true;
	}

	return false;
}

bool Sheep::WantEscape()
{
	if (m_fEscapeTime > 1)
	{
		m_fEscapeTime = 0;
		if (rand_0_1() < 0.10)
		{
			return true;
		}
	}

	return false;
}

bool Sheep::isFull()const
{
	return MaxSatiety == m_satiety;
}

bool Sheep::isMoving()const
{
	return !m_isEndMove;
}

void Sheep::DecreaseSatiety()
{
	if (m_fDecreaseSatietyTime > 1)
	{
		m_fDecreaseSatietyTime = 0;
		if (rand_0_1() < 0.3)
		{
			m_satiety--;
			if (m_satiety < 0)
			{
				m_satiety = 0;
			}
		}
	}
}

void Sheep::IncreaseSatiety()
{
	if (m_fIncreaseSatietyTime > 1)
	{
		m_fIncreaseSatietyTime = 0;
		if (rand_0_1() < 0.3)
		{
			m_satiety++;
			if (m_satiety > 5)
			{
				m_satiety = 5;
			}
		}
	}
}

Sheep::Sheep()
{
}


Sheep::~Sheep()
{
	delete m_pStateMachine;
}

void Sheep::ArriveWithPath(std::list<Vec2> path, bool loop)
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

void Sheep::WanderInFence()
{
	m_pSteering->StopOff();
	m_pSteering->WanderOn();
	m_pSteering->WallAvoidanceOn();
	m_pSteering->FollowPathOff();
	m_pSteering->ArriveOff();
	m_pSteering->OffsetPursuitOff();
	m_pSteering->SeparationOn();
}

void Sheep::Arrive(Vec2 dest)
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

void Sheep::OffsetPursuit(MovingEntity* leader, cocos2d::Vec2 offest)
{
	m_pSteering->StopOff();
	m_pSteering->WallAvoidanceOff();
	m_pSteering->WanderOff();
	m_pSteering->ArriveOff();
	m_pSteering->FollowPathOff();
	m_pSteering->OffsetPursuitOn(leader, offest);
	m_pSteering->SeparationOn();
}

void Sheep::MoveEnd()
{
	m_isEndMove = true;
}

void Sheep::PlayAnimation()
{
	m_isPlayAnimation = true;
	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFileName("sheep.png");
	m_animation->addSpriteFrameWithFileName("sheep1.png");
	m_animation->setDelayPerUnit(0.3);

	m_animate = Animate::create(m_animation);
	m_repeatAnimate = RepeatForever::create(m_animate);

	m_sprite->runAction(m_repeatAnimate);
}

void Sheep::StopAnimation()
{
	m_isPlayAnimation = false;
	if (m_repeatAnimate != NULL)
	{
		m_sprite->stopAction(m_repeatAnimate);
	}

	m_sprite->setTexture("sheep.png");
}

void Sheep::SetSpriteSpeak()
{
	m_sprite->setTexture("sheep1.png");
}
void Sheep::SetSpriteNotSpeak()
{
	m_sprite->setTexture("sheep.png");
}

bool Sheep::IsOutHouse()
{
	return !m_isInHouse;
}

bool Sheep::IsContainedHouse()
{
	return m_isInHouse;
}

void Sheep::SetInHouse()
{
	m_isInHouse = true;
}

void Sheep::SetOutHouse()
{
	m_isInHouse = false;
}