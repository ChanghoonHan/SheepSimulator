#pragma once
#include <cocos2d.h>

class Wall2D;
class MovingEntity;

class GameWorld
{
	static GameWorld* S;

	std::vector<MovingEntity*>    m_Vehicles;
	std::vector<Wall2D>           m_Walls;
	cocos2d::Vec2						  m_vCrosshair;

	int                           m_cxClient, m_cyClient;

	double                        m_dAvFrameTime;

	GameWorld();
	~GameWorld();

	void CreateWalls();
public:
	static GameWorld* GetInstance();
	static void Destroy();

	const std::vector<Wall2D>&          Walls() { return m_Walls; }
	const std::vector<MovingEntity*>&        Agents() { return m_Vehicles; }

	cocos2d::Vec2    Crosshair()const { return m_vCrosshair; }

	void SetVehicles(MovingEntity* movingEntity);

	double GetElapsedTime() { return m_dAvFrameTime; }

	int   cxClient()const { return m_cxClient; }
	int   cyClient()const { return m_cyClient; }

	void init();
	void Update(float ElapsedTime);
};

