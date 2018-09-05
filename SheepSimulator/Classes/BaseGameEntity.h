#pragma once
#include "Locations.h"
#include <cocos2d.h>

struct Telegram;

class BaseGameEntity : public cocos2d::Node
{
protected:
	cocos2d::Sprite* m_sprite;
	cocos2d::Label* m_label;
	
	bool m_isPlayAnimation;

	std::string m_voice;

	int m_id;

	cocos2d::Vec2 m_pos;

	location_type m_Location;

	cocos2d::Size m_visiableSize;

	double    m_dBoundingRadius;

	virtual void SetLabel() = 0;

	std::string GetLocationStringFromType();

public:
	virtual bool init() = 0;

	virtual bool init(cocos2d::Vec2 pos, int id) = 0;

	virtual void Update(double time_elapsed) = 0;

	virtual bool HandleMessage(const Telegram& msg) = 0;

	cocos2d::Vec2		 Pos()const { return m_pos; }
	void         SetPos(cocos2d::Vec2 new_pos);

	double       BRadius()const { return m_dBoundingRadius; }
	void         SetBRadius(double r) { m_dBoundingRadius = r; }
	int          ID()const { return m_id; }
	bool		 IsPlayAnimation() { return m_isPlayAnimation; };

	void SetVoice(std::string voice);

	BaseGameEntity();
	~BaseGameEntity();
};

