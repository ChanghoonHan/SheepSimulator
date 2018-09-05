#pragma once
#include "State.h"

class Sheep;
struct Telegram;

class EnterSheepHouseAndPlay : public State<Sheep>
{
private:

	EnterSheepHouseAndPlay() {}

	//copy ctor and assignment should be private
	EnterSheepHouseAndPlay(const EnterSheepHouseAndPlay&);
	EnterSheepHouseAndPlay& operator=(const EnterSheepHouseAndPlay&);

public:

	//this is a singleton
	static EnterSheepHouseAndPlay* Instance();

	virtual void Enter(Sheep* sheep);

	virtual void Execute(Sheep* sheep);

	virtual void Exit(Sheep* sheep);

	virtual bool OnMessage(Sheep* sheep, const Telegram& msg);
};

class GoFieldAndEat : public State<Sheep>
{
private:

	GoFieldAndEat() {}

	//copy ctor and assignment should be private
	GoFieldAndEat(const GoFieldAndEat&);
	GoFieldAndEat& operator=(const GoFieldAndEat&);

public:

	//this is a singleton
	static GoFieldAndEat* Instance();

	virtual void Enter(Sheep* sheep);

	virtual void Execute(Sheep* sheep);

	virtual void Exit(Sheep* sheep);

	virtual bool OnMessage(Sheep* sheep, const Telegram& msg);

};

class GoDoorAndCallDogForEat : public State<Sheep>
{
private:

	GoDoorAndCallDogForEat() {}

	//copy ctor and assignment should be private
	GoDoorAndCallDogForEat(const GoDoorAndCallDogForEat&);
	GoDoorAndCallDogForEat& operator=(const GoDoorAndCallDogForEat&);

public:

	//this is a singleton
	static GoDoorAndCallDogForEat* Instance();

	virtual void Enter(Sheep* sheep);

	virtual void Execute(Sheep* sheep);

	virtual void Exit(Sheep* sheep);

	virtual bool OnMessage(Sheep* sheep, const Telegram& msg);

};


class GoDoorAndWaitingForEnterHouse : public State<Sheep>
{
private:

	GoDoorAndWaitingForEnterHouse() {}

	//copy ctor and assignment should be private
	GoDoorAndWaitingForEnterHouse(const GoDoorAndWaitingForEnterHouse&);
	GoDoorAndWaitingForEnterHouse& operator=(const GoDoorAndWaitingForEnterHouse&);

public:

	//this is a singleton
	static GoDoorAndWaitingForEnterHouse* Instance();

	virtual void Enter(Sheep* sheep);

	virtual void Execute(Sheep* sheep);

	virtual void Exit(Sheep* sheep);

	virtual bool OnMessage(Sheep* sheep, const Telegram& msg);

};

class MoveOutFenceAndWait : public State<Sheep>
{
private:

	MoveOutFenceAndWait() {}

	//copy ctor and assignment should be private
	MoveOutFenceAndWait(const MoveOutFenceAndWait&);
	MoveOutFenceAndWait& operator=(const MoveOutFenceAndWait&);

public:

	//this is a singleton
	static MoveOutFenceAndWait* Instance();

	virtual void Enter(Sheep* sheep);

	virtual void Execute(Sheep* sheep);

	virtual void Exit(Sheep* sheep);

	virtual bool OnMessage(Sheep* sheep, const Telegram& msg);

};


class EscapeFromFence : public State<Sheep>
{
private:

	EscapeFromFence() {}

	//copy ctor and assignment should be private
	EscapeFromFence(const EscapeFromFence&);
	EscapeFromFence& operator=(const EscapeFromFence&);

public:

	//this is a singleton
	static EscapeFromFence* Instance();

	virtual void Enter(Sheep* sheep);

	virtual void Execute(Sheep* sheep);

	virtual void Exit(Sheep* sheep);

	virtual bool OnMessage(Sheep* sheep, const Telegram& msg);

};

