#include "GameWorld.h"
#include "Wall2D.h"

USING_NS_CC;

GameWorld* GameWorld::S = NULL;

void GameWorld::init()
{
	m_cxClient = Director::getInstance()->getVisibleSize().width;
	m_cyClient = Director::getInstance()->getVisibleSize().height;
	m_vCrosshair = Vec2(m_cxClient / 2.0, m_cxClient / 2.0);

	CreateWalls();
}

void GameWorld::CreateWalls()
{
	const int NumWallVerts = 4;

	Vec2 walls[NumWallVerts] = { Vec2(0, 425),
		Vec2(m_cxClient / 2 -50, 425),
		Vec2(m_cxClient / 2 -50, m_cyClient),
		Vec2(0, m_cyClient) };

	for (int w = 0; w<NumWallVerts - 1; ++w)
	{
		m_Walls.push_back(Wall2D(walls[w], walls[w + 1]));
	}

	m_Walls.push_back(Wall2D(walls[NumWallVerts - 1], walls[0]));
}

void GameWorld::SetVehicles(MovingEntity* movingEntity)
{
	m_Vehicles.push_back(movingEntity);
}

void GameWorld::Update(float ElapsedTime)
{
	m_dAvFrameTime = ElapsedTime;
}

GameWorld* GameWorld::GetInstance()
{
	if (S == NULL)
	{
		S = new GameWorld;
	}

	return S;
}

void GameWorld::Destroy()
{
	if (S != NULL)
	{
		delete S;
	}

	S = NULL;
}

GameWorld::GameWorld()
{
}


GameWorld::~GameWorld()
{
}
