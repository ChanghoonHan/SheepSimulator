#include "FenceDoorStates.h"
#include "FenceDoor.h"
#include "MessageTypes.h"
#include "MessageDispatcher.h"
#include "EntityNames.h"

Closed* Closed::Instance()
{
	static Closed instance;

	return &instance;
}

void Closed::Enter(FenceDoor* door)
{
	if (door->Location() != door_closed)
	{
		door->ChangeLocation(door_closed);
	}
}

void Closed::Execute(FenceDoor* door)
{
	
}

void Closed::Exit(FenceDoor* door)
{
	//go field
}

bool Closed::OnMessage(FenceDoor* door, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_OpenTheDoor:
	{
		door->SetDestinationLocation(door_opened);
		door->GetFSM()->ChangeState(MoveFence::Instance());
	}

	return true;

	}//end switch

	return false;
}


Opened* Opened::Instance()
{
	static Opened instance;

	return &instance;
}

void Opened::Enter(FenceDoor* door)
{
	if (door->Location() != door_opened)
	{
		door->ChangeLocation(door_opened);
	}

	for (size_t i = 0; i < 5; i++)
	{
		MessageDispatcher::Instance()->DispatchMsg(door->ID(), i, Msg_DoorIsOpen, NO_ADDITIONAL_INFO);
	}

}

void Opened::Execute(FenceDoor* door)
{

}

void Opened::Exit(FenceDoor* door)
{
	//go field
}

bool Opened::OnMessage(FenceDoor* door, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_CloseTheDoor:
	{
		door->SetDestinationLocation(door_closed);
		door->GetFSM()->ChangeState(MoveFence::Instance());
	}

	return true;

	}//end switch

	return false;
}


MoveFence* MoveFence::Instance()
{
	static MoveFence instance;

	return &instance;
}

void MoveFence::Enter(FenceDoor* door)
{
	if (door->Location() != moving)
	{
		door->ChangeLocation(moving);
	}

	switch (door->GetDestinationLocation())
	{
	case door_closed:
		door->Close();
		break;
	case door_opened:
		door->Open();
		break;
	default:
		break;
	}
}

void MoveFence::Execute(FenceDoor* door)
{
	if (!door->isMoving())
	{
		switch (door->GetDestinationLocation())
		{
		case door_closed:
			door->GetFSM()->ChangeState(Closed::Instance());
			break;
		case door_opened:
			door->GetFSM()->ChangeState(Opened::Instance());
			break;

		default:
			break;
		}
	}
}

void MoveFence::Exit(FenceDoor* door)
{
	//go field
}

bool MoveFence   ::OnMessage(FenceDoor* door, const Telegram& msg)
{

	return false;
}
