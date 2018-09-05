#include "SheepSimulator.h"
#include "SimpleAudioEngine.h"
#include "../proj.win32/Sheep.h"
#include "../proj.win32/Dog.h"
#include "../proj.win32/FenceDoor.h"
#include "../proj.win32/EntityNames.h"
#include "../proj.win32/EntityManager.h"
#include "../proj.win32/GameWorld.h"

USING_NS_CC;

Scene* SheepSimulator::createScene()
{
    return SheepSimulator::create();
}

// on "init" you need to initialize your instance
bool SheepSimulator::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	GameWorld::GetInstance()->init();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* backGround = Sprite::create("BackGround.png");
	backGround->setAnchorPoint(Vec2(0, 0));
	backGround->setPosition(Vec2(0, 0));
	addChild(backGround,-3);

	Sprite* field = Sprite::create("Field.png");
	field->setAnchorPoint(Vec2(1, 1));
	field->setPosition(Vec2(visibleSize.width, visibleSize.height));
	addChild(field, -2);

	for (size_t i = 0; i < MAXSHEEP; i++)
	{
		m_sheep[i] = Sheep::create();

		if (i < 3)
		{
			m_sheep[i]->init(Vec2(200 + i * 150, visibleSize.height - 200), i);
		}
		else
		{
			m_sheep[i]->init(Vec2(200 + ((i - 2) * 150), visibleSize.height - 400), i);
		}

		EntityManager::Instance()->RegisterEntity(m_sheep[i]);
		addChild(m_sheep[i]);
		GameWorld::GetInstance()->SetVehicles(m_sheep[i]);
	}
	
	m_fenceDoor = FenceDoor::create();
	m_fenceDoor->init(Vec2(204, 425), ent_fenceDoor);

	EntityManager::Instance()->RegisterEntity(m_fenceDoor);

	addChild(m_fenceDoor);

	scheduleUpdate();

	for (size_t i = 0; i < 20; i++)
	{
		m_fences[i] = Sprite::create("fence.png");
	}

	for (size_t i = 0; i < 8; i++)
	{
		if (i == 2)
		{
			continue;
		}
		m_fences[i]->setPosition(Vec2(0 + (i * 102), 425));
		addChild(m_fences[i]);
	}

	for (size_t i = 8; i < 16; i++)
	{
		m_fences[i]->setRotation(-90);
		m_fences[i]->setPosition(Vec2(visibleSize.width/2-20, visibleSize.height/2 + ((i - 8) * 102)));
		addChild(m_fences[i]);
	}

	m_dog[0] = Dog::create();
	m_dog[0]->init(Vec2(200, 50), ent_dog1);
	addChild(m_dog[0]);
	EntityManager::Instance()->RegisterEntity(m_dog[0]);

	m_dog[1] = Dog::create();
	m_dog[1]->init(Vec2(200, 50), ent_dog2);
	addChild(m_dog[1]);
	EntityManager::Instance()->RegisterEntity(m_dog[1]);

	m_dogHouse = Sprite::create("DogHouse.png");
	m_dogHouse->setAnchorPoint(Vec2(0, 0));
	m_dogHouse->setPosition(Vec2(0, 0));
	addChild(m_dogHouse, -1);
    return true;
}

void SheepSimulator::update(float deltaTime)
{
	GameWorld::GetInstance()->Update(deltaTime);
	for (size_t i = 0; i < MAXSHEEP; i++)
	{
		m_sheep[i]->Update(deltaTime);
	}
	
	for (size_t i = 0; i < 2; i++)
	{
		m_dog[i]->Update(deltaTime);
	}

	m_fenceDoor->Update(deltaTime);
}
