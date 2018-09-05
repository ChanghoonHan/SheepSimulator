#include "BaseGameEntity.h"

USING_NS_CC;

std::string BaseGameEntity::GetLocationStringFromType()
{
	switch (m_Location)
	{
	case dogHouse:
		return "DogHouse";
		break;

	case sheepHouse:
		return "SheepHouse";
		break;

	case field:
		return "Field";
		break;

	case moving:
		return "Moving";
		break;

	case waiting:
		return "Waiting";
		break;

	case fenceDoorInside:
		return "FenceDoorInSide";
		break;

	case fenceDoorOutside:
		return "FenceDoorOutside";
		break;

	case fencePatrol:
		return "FencePatrol";
		break;

	case door_closed:
		return "Closed";
		break;

	case door_opened:
		return "Opened";
		break;

	default:
		return "default";
		break;
	}
}


void BaseGameEntity::SetPos(cocos2d::Vec2 new_pos)
{
	this->setPosition(new_pos);
	m_sprite->setPosition(0, 0);
	m_label->setPosition(0, 70);
	m_label->setTextColor(Color4B::BLACK);
}

void BaseGameEntity::SetVoice(std::string voice)
{
	m_voice = voice;
}

BaseGameEntity::BaseGameEntity()
{
	m_voice = "";
}


BaseGameEntity::~BaseGameEntity()
{
}
