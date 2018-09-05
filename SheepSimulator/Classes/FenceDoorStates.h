#pragma once
#include "State.h"

class FenceDoor;
struct Telegram;

class Closed : public State<FenceDoor>
{
	Closed() {}

	//copy ctor and assignment should be private
	Closed(const Closed&);
	Closed& operator=(const Closed&);

public:

	//this is a singleton
	static Closed* Instance();

	virtual void Enter(FenceDoor* door);

	virtual void Execute(FenceDoor* door);

	virtual void Exit(FenceDoor* door);

	virtual bool OnMessage(FenceDoor* door, const Telegram& msg);
};

class Opened : public State<FenceDoor>
{
	Opened() {}

	//copy ctor and assignment should be private
	Opened(const Opened&);
	Opened& operator=(const Opened&);

public:

	//this is a singleton
	static Opened* Instance();

	virtual void Enter(FenceDoor* door);

	virtual void Execute(FenceDoor* door);

	virtual void Exit(FenceDoor* door);

	virtual bool OnMessage(FenceDoor* door, const Telegram& msg);
};

class MoveFence : public State<FenceDoor>
{
	MoveFence() {}

	//copy ctor and assignment should be private
	MoveFence(const MoveFence&);
	MoveFence& operator=(const MoveFence&);

public:

	//this is a singleton
	static MoveFence* Instance();

	virtual void Enter(FenceDoor* door);

	virtual void Execute(FenceDoor* door);

	virtual void Exit(FenceDoor* door);

	virtual bool OnMessage(FenceDoor* door, const Telegram& msg);
};