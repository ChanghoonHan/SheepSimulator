#include "FenceDoor.h"
#include "FenceDoorStates.h"

USING_NS_CC;

void FenceDoor::SetLabel()
{
	m_label->setString(StringUtils::format("Loc : %s\nVoice : %s", GetLocationStringFromType().c_str(), m_voice.c_str()));
}

bool FenceDoor::init()
{
	m_isEndMove = true;
	m_visiableSize = Director::getInstance()->getVisibleSize();
	m_sprite = Sprite::create("fence.png");
	m_label = Label::createWithSystemFont("", "Thonburi", 15);
	SetLabel();
	addChild(m_sprite);
	addChild(m_label);
	SetPos(Vec2(0, 0));
	return true;
}

bool FenceDoor::init(Vec2 pos, int id)
{
	m_id = id;

	m_pStateMachine = new StateMachine<FenceDoor>(this);

	m_pStateMachine->SetCurrentState(Closed::Instance());

	m_Location = door_closed;

	m_pos = pos;
	SetPos(m_pos);

	return true;
}

void FenceDoor::Update(double deltatime)
{
	m_pStateMachine->Update();

	SetLabel();
}

bool FenceDoor::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

bool FenceDoor::isMoving()const
{
	return !m_isEndMove;
}

void FenceDoor::Open()
{
	m_isEndMove = false;
	MoveBy* moveBy1 = MoveBy::create(2, Vec2(90, 0));
	CallFunc* endCallBackFunk = CallFunc::create(CC_CALLBACK_0(FenceDoor::MoveEndCallBackFunc, this));

	this->runAction(Sequence::create(moveBy1, endCallBackFunk, NULL));
}

void FenceDoor::Close()
{
	m_isEndMove = false;
	MoveBy* moveBy1 = MoveBy::create(2, Vec2(-90, 0));
	CallFunc* endCallBackFunk = CallFunc::create(CC_CALLBACK_0(FenceDoor::MoveEndCallBackFunc, this));

	this->runAction(Sequence::create(moveBy1, endCallBackFunk, NULL));
}

void FenceDoor::MoveEndCallBackFunc()
{
	m_isEndMove = true;
}

FenceDoor::FenceDoor()
{
}


FenceDoor::~FenceDoor()
{
	delete m_pStateMachine;
}
