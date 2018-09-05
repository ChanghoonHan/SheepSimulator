#pragma once
#ifndef __SHEEPSIMULATOR_SCENE_H__
#define __SHEEPSIMULATOR_SCENE_H__

#include <cocos2d.h>

#define MAXSHEEP 5

class Sheep;
class Dog;
class FenceDoor;


class SheepSimulator : public cocos2d::Scene
{
	Sheep* m_sheep[MAXSHEEP];
	Dog* m_dog[2];
	FenceDoor* m_fenceDoor;
	cocos2d::Sprite* m_fences[20];
	cocos2d::Sprite* m_dogHouse;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SheepSimulator);

	void update(float deltaTime);
};

#endif // __SHEEPSIMULATOR_SCENE_H__
