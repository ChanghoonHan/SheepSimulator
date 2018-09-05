#include "SheepStates.h"
#include "Sheep.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"
#include "EntityManager.h"

EnterSheepHouseAndPlay* EnterSheepHouseAndPlay::Instance()
{
	static EnterSheepHouseAndPlay instance;

	return &instance;
}

void EnterSheepHouseAndPlay::Enter(Sheep* sheep)
{
	if (sheep->GetFSM()->PreviousState() != NULL &&
		(sheep->GetFSM()->PreviousState() == EscapeFromFence::Instance()))
	{
		sheep->Arrive(cocos2d::Vec2(200, 600));
	}

	if (sheep->GetFSM()->PreviousState() != NULL &&
		(sheep->GetFSM()->PreviousState() == GoDoorAndWaitingForEnterHouse::Instance() ||
			sheep->GetFSM()->PreviousState() == GoFieldAndEat::Instance()))
	{
		std::list<cocos2d::Vec2> path;
		path = { { 200, 300 }, { 200, 600 } };
		sheep->ArriveWithPath(path);
	}
}

void EnterSheepHouseAndPlay::Execute(Sheep* sheep)
{
	if (!sheep->isMoving())
	{
		if (sheep->Location() != sheepHouse)
		{
			sheep->ChangeLocation(sheepHouse);
		}
		sheep->SetInHouse();
		sheep->WanderInFence();

		sheep->DecreaseSatiety();

		if (!sheep->ishungry() && sheep->WantEscape())
		{
			sheep->GetFSM()->ChangeState(EscapeFromFence::Instance());
		}

		if (sheep->ishungry() && sheep->ID() == ent_sheep_boss)
		{
			if (EntityManager::Instance()->IsSheepAllHungry())
			{
				sheep->GetFSM()->ChangeState(GoDoorAndCallDogForEat::Instance());
			}
		}
	}
}

void EnterSheepHouseAndPlay::Exit(Sheep* sheep)
{
	if (sheep->Location() != moving)
	{
		sheep->ChangeLocation(moving);
	}
	//go field
}

bool EnterSheepHouseAndPlay::OnMessage(Sheep* sheep, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_ComeOut:
		sheep->GetFSM()->ChangeState(MoveOutFenceAndWait::Instance());
		break;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

MoveOutFenceAndWait* MoveOutFenceAndWait::Instance()
{
	static MoveOutFenceAndWait instance;

	return &instance;
}

void MoveOutFenceAndWait::Enter(Sheep* sheep)
{
	std::list<cocos2d::Vec2> path;
	if (sheep->ID() == ent_sheep_boss)
	{
		path = { { 200, 450 },{ 200, 300 },{ 400, 200 } };

		for (size_t i = 1; i < 5; i++)
		{
			MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), i, Msg_ComeOut, NO_ADDITIONAL_INFO);
		}
	}
	else
	{
		path = { { 200, 450 },{ 200, 300 },{ 300, 200 } };
	}
	sheep->ArriveWithPath(path);
}

void MoveOutFenceAndWait::Execute(Sheep* sheep)
{
	if (!sheep->isMoving())
	{
		if (sheep->Location() != waiting)
		{
			sheep->ChangeLocation(waiting);
		}
		sheep->SetOutHouse();

		if (sheep->ID() != ent_sheep_boss)
		{
			sheep->OffsetPursuit(dynamic_cast <Sheep*> (EntityManager::Instance()->GetEntityFromID(ent_sheep_boss)), cocos2d::Vec2(50, 50));
		}
		else
		{
			if (EntityManager::Instance()->IsSheepAllOutFromHouse())
			{
				sheep->GetFSM()->ChangeState(GoFieldAndEat::Instance());
			}
		}
	}
}

void MoveOutFenceAndWait::Exit(Sheep* sheep)
{
	if (sheep->Location() != moving)
	{
		sheep->ChangeLocation(moving);
	}
}

bool MoveOutFenceAndWait::OnMessage(Sheep* sheep, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_EatStart:
		sheep->GetFSM()->ChangeState(GoFieldAndEat::Instance());
		break;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

GoFieldAndEat* GoFieldAndEat::Instance()
{
	static GoFieldAndEat instance;

	return &instance;
}

void GoFieldAndEat::Enter(Sheep* sheep)
{
	if (sheep->ID() == ent_sheep_boss)
	{
		std::list<cocos2d::Vec2> path;

		path = { { 1000, 200 },{ 1100, 400 },{ 1200, 500 } };

		sheep->ArriveWithPath(path);
	}
	else
	{
		switch (sheep->ID())
		{
		case ent_sheep1:
			sheep->Arrive(cocos2d::Vec2(950, 700));
			break;
		case ent_sheep2:
			sheep->Arrive(cocos2d::Vec2(1300, 800));
			break;
		case ent_sheep3:
			sheep->Arrive(cocos2d::Vec2(900, 300));
			break;
		case ent_sheep4:
			sheep->Arrive(cocos2d::Vec2(1200, 250));
			break;
		default:
			break;
		}
	}
}

void GoFieldAndEat::Execute(Sheep* sheep)
{
	if (!sheep->isMoving())
	{	
		if (sheep->isFull())
		{
			if (sheep->Location() != moving)
			{
				sheep->StopAnimation();
				sheep->ChangeLocation(moving);
				if (sheep->ID() == ent_sheep_boss)
				{
					std::list<cocos2d::Vec2> path;
					path = { { 950, 700 },{ 1300, 800 },{ 1200, 250 },{ 900, 300 } };
					sheep->ArriveWithPath(path, true);
				}
				else
				{
					sheep->OffsetPursuit(dynamic_cast <Sheep*> (EntityManager::Instance()->GetEntityFromID(ent_sheep_boss)), cocos2d::Vec2(50, 50));
				}
			}
		}
		else
		{
			if (sheep->Location() != field)
			{
				sheep->ChangeLocation(field);
			}

			if (!sheep->IsPlayAnimation())
			{
				sheep->PlayAnimation();

				if (sheep->ID() == ent_sheep_boss)
				{
					for (size_t i = 1; i < 7; i++)
					{
						MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), i, Msg_EatStart, NO_ADDITIONAL_INFO);
					}
				}
			}

			sheep->IncreaseSatiety();
		}
	}


	if (sheep->ID() == ent_sheep_boss && sheep->isFull())
	{
		sheep->StopAnimation();

		if (EntityManager::Instance()->IsSheepAllFull())
		{
			sheep->GetFSM()->ChangeState(GoDoorAndWaitingForEnterHouse::Instance());
		}
	}
}

void GoFieldAndEat::Exit(Sheep* sheep)
{
	if (sheep->Location() != moving)
	{
		sheep->ChangeLocation(moving);
	}
}

bool GoFieldAndEat::OnMessage(Sheep* sheep, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_DoorIsOpen:
	{
		sheep->GetFSM()->ChangeState(EnterSheepHouseAndPlay::Instance());
	}

	return true;

	}//end switch

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

GoDoorAndCallDogForEat* GoDoorAndCallDogForEat::Instance()
{
	static GoDoorAndCallDogForEat instance;

	return &instance;
}

void GoDoorAndCallDogForEat::Enter(Sheep* sheep)
{
	sheep->Arrive(cocos2d::Vec2(200, 450));
}

void GoDoorAndCallDogForEat::Execute(Sheep* sheep)
{
	if (!sheep->isMoving())
	{
		if (sheep->Location() != fenceDoorInside)
		{
			sheep->ChangeLocation(fenceDoorInside);
		}
		sheep->SetSpriteSpeak();
		sheep->SetVoice("Baa!!!");
		MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog1, Msg_WeAreHungry, NO_ADDITIONAL_INFO);
		MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog2, Msg_WeAreHungry, NO_ADDITIONAL_INFO);
	}

	//mee Ãâ·Â
}

void GoDoorAndCallDogForEat::Exit(Sheep* sheep)
{
	sheep->SetSpriteNotSpeak();
	sheep->SetVoice("");
	if (sheep->Location() != moving)
	{
		sheep->ChangeLocation(moving);
	}
}

bool GoDoorAndCallDogForEat::OnMessage(Sheep* sheep, const Telegram& msg)
{
	switch (msg.Msg)

	{
	case Msg_DoorIsOpen:
	{
		sheep->GetFSM()->ChangeState(MoveOutFenceAndWait::Instance());
	}

	return true;

	}//end switch

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

GoDoorAndWaitingForEnterHouse* GoDoorAndWaitingForEnterHouse::Instance()
{
	static GoDoorAndWaitingForEnterHouse instance;

	return &instance;
}

void GoDoorAndWaitingForEnterHouse::Enter(Sheep* sheep)
{
	std::list<cocos2d::Vec2> path;

	path = { { 1200, 500 }, { 1100, 400 },{ 1000, 150 } ,{ 400, 150 } };
	MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog1, Msg_WeGoHoom, NO_ADDITIONAL_INFO);
	MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog2, Msg_WeGoHoom, NO_ADDITIONAL_INFO);
	sheep->ArriveWithPath(path);
}

void GoDoorAndWaitingForEnterHouse::Execute(Sheep* sheep)
{
	if (!sheep->isMoving())
	{
		if (sheep->Location() != fenceDoorOutside)
		{
			sheep->ChangeLocation(fenceDoorOutside);
		}

		sheep->SetSpriteSpeak();
		sheep->SetVoice("Baa!!!");

		MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog1, Msg_WeAreHoom, NO_ADDITIONAL_INFO);
		MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog2, Msg_WeAreHoom, NO_ADDITIONAL_INFO);
	}

}

void GoDoorAndWaitingForEnterHouse::Exit(Sheep* sheep)
{
	sheep->SetSpriteNotSpeak();
	sheep->SetVoice("");
}

bool GoDoorAndWaitingForEnterHouse::OnMessage(Sheep* sheep, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_DoorIsOpen:
	{
		sheep->GetFSM()->ChangeState(EnterSheepHouseAndPlay::Instance());
	}

	return true;

	}//end switch

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

EscapeFromFence* EscapeFromFence::Instance()
{
	static EscapeFromFence instance;

	return &instance;
}

void EscapeFromFence::Enter(Sheep* sheep)
{
	float pos;
	if (cocos2d::rand_0_1() < 0.5)
	{
		pos = 850 - (cocos2d::rand_0_1() * 500);
		sheep->Arrive(cocos2d::Vec2(900, pos));
	}
	else
	{
		pos = 100 + (cocos2d::rand_0_1() * 800);
		sheep->Arrive(cocos2d::Vec2(pos, 350));
	}
}

void EscapeFromFence::Execute(Sheep* sheep)
{
	if (!sheep->isMoving())
	{
		sheep->SetSpriteSpeak();
		sheep->SetVoice("Baa");

		MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog1, Msg_IEscaped, NO_ADDITIONAL_INFO);
		MessageDispatcher::Instance()->DispatchMsg(sheep->ID(), ent_dog2, Msg_IEscaped, NO_ADDITIONAL_INFO);
	}

}

void EscapeFromFence::Exit(Sheep* sheep)
{
	sheep->SetSpriteNotSpeak();
	sheep->SetVoice("");
	if (sheep->Location() != moving)
	{
		sheep->ChangeLocation(moving);
	}
}

bool EscapeFromFence::OnMessage(Sheep* sheep, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_PlzGoIntoYourHouse:
	{
		sheep->GetFSM()->ChangeState(EnterSheepHouseAndPlay::Instance());
	}

	return true;

	}//end switch

	return false;
}