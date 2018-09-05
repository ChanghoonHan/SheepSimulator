#pragma once
#include <map>
#include <vector>
#include <cassert>
#include <string>


class BaseGameEntity;
class MovingEntity;

//provide easy access
#define EntityMgr EntityManager::Instance()



class EntityManager
{
private:

	typedef std::map<int, BaseGameEntity*> EntityMap;

private:

	//to facilitate quick lookup the entities are stored in a std::map, in which
	//pointers to entities are cross referenced by their identifying number
	EntityMap m_EntityMap;

	EntityManager() {}

	//copy ctor and assignment should be private
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);

	std::string GetLocationStringFromType();

public:

	static EntityManager* Instance();

	//this method stores a pointer to the entity in the std::vector
	//m_Entities at the index position indicated by the entity's ID
	//(makes for faster access)
	void            RegisterEntity(BaseGameEntity* NewEntity);

	//returns a pointer to the entity with the ID given as a parameter
	BaseGameEntity* GetEntityFromID(int id)const;

	//this method removes the entity from the list
	void            RemoveEntity(BaseGameEntity* pEntity);

	bool			IsSheepAllHungry();
	bool			IsSheepAllFull();
	bool			IsSheepAllOutFromHouse();
	bool			IsHouseContainedAllSheep();
	MovingEntity*	GetSheepBoss();
};
