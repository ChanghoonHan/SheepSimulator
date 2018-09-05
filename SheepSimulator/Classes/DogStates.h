#pragma once
#include "State.h"

class Dog;
struct Telegram;

class GlobalState : public State<Dog>
{
	GlobalState() {}

	//copy ctor and assignment should be private
	GlobalState(const GlobalState&);
	GlobalState& operator=(const GlobalState&);

public:

	//this is a singleton
	static GlobalState* Instance();

	virtual void Enter(Dog* dog);

	virtual void Execute(Dog* dog);

	virtual void Exit(Dog* dog);

	virtual bool OnMessage(Dog* dog, const Telegram& msg);
};

class GoHoomAndTakeARest : public State<Dog>
{
	GoHoomAndTakeARest() {}

	//copy ctor and assignment should be private
	GoHoomAndTakeARest(const GoHoomAndTakeARest&);
	GoHoomAndTakeARest& operator=(const GoHoomAndTakeARest&);

public:

	//this is a singleton
	static GoHoomAndTakeARest* Instance();

	virtual void Enter(Dog* dog);

	virtual void Execute(Dog* dog);

	virtual void Exit(Dog* dog);

	virtual bool OnMessage(Dog* dog, const Telegram& msg);
};

class GoFenceDoorAndOpen : public State<Dog>
{

	GoFenceDoorAndOpen() {}

	//copy ctor and assignment should be private
	GoFenceDoorAndOpen(const GoFenceDoorAndOpen&);
	GoFenceDoorAndOpen& operator=(const GoFenceDoorAndOpen&);

public:

	//this is a singleton
	static GoFenceDoorAndOpen* Instance();

	virtual void Enter(Dog* dog);

	virtual void Execute(Dog* dog);

	virtual void Exit(Dog* dog);

	virtual bool OnMessage(Dog* dog, const Telegram& msg);
};

class GoFieldAndPatrol : public State<Dog>
{

	GoFieldAndPatrol() {}

	//copy ctor and assignment should be private
	GoFieldAndPatrol(const GoFieldAndPatrol&);
	GoFieldAndPatrol& operator=(const GoFieldAndPatrol&);

public:

	//this is a singleton
	static GoFieldAndPatrol* Instance();

	virtual void Enter(Dog* dog);

	virtual void Execute(Dog* dog);

	virtual void Exit(Dog* dog);

	virtual bool OnMessage(Dog* dog, const Telegram& msg);
};

class GoFenceAndPatrol : public State<Dog>
{

	GoFenceAndPatrol() {}

	//copy ctor and assignment should be private
	GoFenceAndPatrol(const GoFenceAndPatrol&);
	GoFenceAndPatrol& operator=(const GoFenceAndPatrol&);

public:

	//this is a singleton
	static GoFenceAndPatrol* Instance();

	virtual void Enter(Dog* dog);

	virtual void Execute(Dog* dog);

	virtual void Exit(Dog* dog);

	virtual bool OnMessage(Dog* dog, const Telegram& msg);
};


class GoSheepAndWoof : public State<Dog>
{

	GoSheepAndWoof() {}

	//copy ctor and assignment should be private
	GoSheepAndWoof(const GoSheepAndWoof&);
	GoSheepAndWoof& operator=(const GoSheepAndWoof&);

public:

	//this is a singleton
	static GoSheepAndWoof* Instance();

	virtual void Enter(Dog* dog);

	virtual void Execute(Dog* dog);

	virtual void Exit(Dog* dog);

	virtual bool OnMessage(Dog* dog, const Telegram& msg);
};

class ArriveFenceForPatrol : public State<Dog>
{

	ArriveFenceForPatrol() {}

	//copy ctor and assignment should be private
	ArriveFenceForPatrol(const ArriveFenceForPatrol&);
	ArriveFenceForPatrol& operator=(const ArriveFenceForPatrol&);

public:

	//this is a singleton
	static ArriveFenceForPatrol* Instance();

	virtual void Enter(Dog* dog);

	virtual void Execute(Dog* dog);

	virtual void Exit(Dog* dog);

	virtual bool OnMessage(Dog* dog, const Telegram& msg);
};
