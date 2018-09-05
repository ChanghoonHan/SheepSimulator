#pragma once
#include "DogStates.h"
#include "Dog.h"
#include "Sheep.h"
#include "MessageTypes.h"
#include "MessageDispatcher.h"
#include "EntityNames.h"
#include "EntityManager.h"

GlobalState* GlobalState::Instance()
{
	static GlobalState instance;

	return &instance;
}

void GlobalState::Enter(Dog* dog)
{

}

void GlobalState::Execute(Dog* dog)
{
	dog->IncreaseTired();
}

void GlobalState::Exit(Dog* dog)
{

}

bool GlobalState::OnMessage(Dog* dog, const Telegram& msg)
{

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GoHoomAndTakeARest* GoHoomAndTakeARest::Instance()
{
	static GoHoomAndTakeARest instance;

	return &instance;
}

void GoHoomAndTakeARest::Enter(Dog* dog)
{
	if (dog->Pos().y < 405)
	{
		dog->Arrive(cocos2d::Vec2(200, 50));
	}
	else
	{
		std::list<cocos2d::Vec2> path;
		path = { { 900, 400 },{ 200, 50 } };
		dog->ArriveWithPath(path);
	}
}

void GoHoomAndTakeARest::Execute(Dog* dog)
{
	dog->DecreaseTired();
	if (!dog->isMoving())
	{
		if (dog->Location() != dogHouse)
		{
			dog->ChangeLocation(dogHouse);
		}
	}
}

void GoHoomAndTakeARest::Exit(Dog* dog)
{
	if (dog->Location() != moving)
	{
		dog->ChangeLocation(moving);
	}
	//go field
}

bool GoHoomAndTakeARest::OnMessage(Dog* dog, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_ImTired:
	{
		dog->GetFSM()->ChangeState(GoFenceAndPatrol::Instance());
	}

	return true;

	}//end switch

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GoFenceDoorAndOpen* GoFenceDoorAndOpen::Instance()
{
	static GoFenceDoorAndOpen instance;

	return &instance;
}

void GoFenceDoorAndOpen::Enter(Dog* dog)
{
	if (dog->Pos().y < 405)
	{
		dog->Arrive(cocos2d::Vec2(200, 400));
	}
	else
	{
		std::list<cocos2d::Vec2> path;
		path = { { 900, 400 }, { 200, 400 } };
		dog->ArriveWithPath(path);
	}
}

void GoFenceDoorAndOpen::Execute(Dog* dog)
{
	if (!dog->isMoving())
	{
		if (dog->Location() != fenceDoorOutside)
		{
			dog->ChangeLocation(fenceDoorOutside);
		}
		MessageDispatcher::Instance()->DispatchMsg(dog->ID(), ent_fenceDoor, Msg_OpenTheDoor, NO_ADDITIONAL_INFO);

		if (dog->GetFSM()->PreviousState() != NULL && dog->GetFSM()->PreviousState() == GoFenceAndPatrol::Instance())
		{
			if (EntityManager::Instance()->IsSheepAllOutFromHouse())
			{
				MessageDispatcher::Instance()->DispatchMsg(dog->ID(), ent_fenceDoor, Msg_CloseTheDoor, NO_ADDITIONAL_INFO);
				dog->GetFSM()->ChangeState(GoFieldAndPatrol::Instance());
			}
		}
		else
		{
			if (EntityManager::Instance()->IsHouseContainedAllSheep())
			{
				MessageDispatcher::Instance()->DispatchMsg(dog->ID(), ent_fenceDoor, Msg_CloseTheDoor, NO_ADDITIONAL_INFO);
				dog->GetFSM()->ChangeState(GoFenceAndPatrol::Instance());
			}
		}
	}
}

void GoFenceDoorAndOpen::Exit(Dog* dog)
{
	if (dog->Location() != moving)
	{
		dog->ChangeLocation(moving);
	}
	//go field
}

bool GoFenceDoorAndOpen::OnMessage(Dog* dog, const Telegram& msg)
{

	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GoFieldAndPatrol* GoFieldAndPatrol::Instance()
{
	static GoFieldAndPatrol instance;

	return &instance;
}

void GoFieldAndPatrol::Enter(Dog* dog)
{
	dog->OffsetPursuit(dynamic_cast <Sheep*> (EntityManager::Instance()->GetEntityFromID(ent_sheep_boss)), cocos2d::Vec2(50, 50));
}

void GoFieldAndPatrol::Execute(Dog* dog)
{
	if (!dog->isMoving())
	{
		if (dog->Location() != field)
		{
			dog->ChangeLocation(field);
		}

	}
}

void GoFieldAndPatrol::Exit(Dog* dog)
{
	if (dog->Location() != moving)
	{
		dog->ChangeLocation(moving);
	}
}

bool GoFieldAndPatrol::OnMessage(Dog* dog, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_WeAreHoom:
	{
		dog->GetFSM()->ChangeState(GoFenceDoorAndOpen::Instance());
	}
	break;

	case Msg_WeGoHoom:
	{
		dog->OffsetPursuit(dynamic_cast <Sheep*> (EntityManager::Instance()->GetEntityFromID(ent_sheep_boss)), cocos2d::Vec2(50, 50));
	}
	break;

	case Msg_EatStart:
	{
		std::list<cocos2d::Vec2> path;
		path = { { 950, 700 },{ 1300, 800 },{ 900, 300 },{ 1300, 250 } };
		dog->ArriveWithPath(path, true);
	}
	break;

	return true;

	}//end switch

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GoFenceAndPatrol* GoFenceAndPatrol::Instance()
{
	static GoFenceAndPatrol instance;

	return &instance;
}

void GoFenceAndPatrol::Enter(Dog* dog)
{
	std::list<cocos2d::Vec2> path;
	path = { { 50, 400 },{ 900, 400 },{ 900, 850 },{ 900, 400 } };
	dog->ArriveWithPath(path, true);

	if (dog->Location() != fencePatrol)
	{
		dog->ChangeLocation(fencePatrol);
	}
}

void GoFenceAndPatrol::Execute(Dog* dog)
{
	if (dog->GetTired() > 50)
	{
		if (dog->ID() == ent_dog1)
		{
			MessageDispatcher::Instance()->DispatchMsg(dog->ID(), ent_dog2, Msg_ImTired, NO_ADDITIONAL_INFO);
		}
		else
		{
			MessageDispatcher::Instance()->DispatchMsg(dog->ID(), ent_dog1, Msg_ImTired, NO_ADDITIONAL_INFO);
		}
		dog->GetFSM()->ChangeState(GoHoomAndTakeARest::Instance());
	}
}

void GoFenceAndPatrol::Exit(Dog* dog)
{
	if (dog->Location() != moving)
	{
		dog->ChangeLocation(moving);
	}
}

bool GoFenceAndPatrol::OnMessage(Dog* dog, const Telegram& msg)
{

	switch (msg.Msg)
	{
	case Msg_WeAreHungry:
	{
		dog->GetFSM()->ChangeState(GoFenceDoorAndOpen::Instance());
	}
	return true;
	
	case Msg_IEscaped:
	{
		dog->SetEscapeSheepId(msg.Sender);
		dog->GetFSM()->ChangeState(GoSheepAndWoof::Instance());
	}
	return true;
	
	}//end switch

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GoSheepAndWoof* GoSheepAndWoof::Instance()
{
	static GoSheepAndWoof instance;

	return &instance;
}

void GoSheepAndWoof::Enter(Dog* dog)
{
	cocos2d::Vec2 escapeSheepPos = EntityManager::Instance()->GetEntityFromID(dog->GetEscapeSheepID())->Pos();

	if (escapeSheepPos.y < 405 && dog->Pos().y < 405)
	{
		dog->Arrive(cocos2d::Vec2(escapeSheepPos.x, escapeSheepPos.y - 50));
	}

	if (escapeSheepPos.y < 405 && dog->Pos().y > 405)
	{
		std::list<cocos2d::Vec2> path;
		path = { { 900, 400 },{ escapeSheepPos.x, escapeSheepPos.y - 50 } };
		dog->ArriveWithPath(path);
	}

	if (escapeSheepPos.y > 405 && dog->Pos().y > 405)
	{
		dog->Arrive(cocos2d::Vec2(escapeSheepPos.x + 50, escapeSheepPos.y));
	}

	if (escapeSheepPos.y > 405 && dog->Pos().y < 405)
	{
		std::list<cocos2d::Vec2> path;
		path = { { 900, 400 },{ escapeSheepPos.x + 50, escapeSheepPos.y } };
		dog->ArriveWithPath(path);
	}
}

void GoSheepAndWoof::Execute(Dog* dog)
{
	if (!dog->isMoving())
	{
		dog->SetVoice("Woof!!!");
		MessageDispatcher::Instance()->DispatchMsg(dog->ID(), dog->GetEscapeSheepID(), Msg_PlzGoIntoYourHouse, NO_ADDITIONAL_INFO);
		dog->GetFSM()->ChangeState(ArriveFenceForPatrol::Instance());
	}
}

void GoSheepAndWoof::Exit(Dog* dog)
{
	
}

bool GoSheepAndWoof::OnMessage(Dog* dog, const Telegram& msg)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ArriveFenceForPatrol* ArriveFenceForPatrol::Instance()
{
	static ArriveFenceForPatrol instance;

	return &instance;
}

void ArriveFenceForPatrol::Enter(Dog* dog)
{
	dog->Arrive(cocos2d::Vec2(900, 400));
}

void ArriveFenceForPatrol::Execute(Dog* dog)
{
	if (!dog->isMoving())
	{
		dog->GetFSM()->ChangeState(GoFenceAndPatrol::Instance());
	}
}

void ArriveFenceForPatrol::Exit(Dog* dog)
{
	dog->SetVoice("");
}

bool ArriveFenceForPatrol::OnMessage(Dog* dog, const Telegram& msg)
{
	return false;
}