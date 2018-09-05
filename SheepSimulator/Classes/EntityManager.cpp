#include "EntityManager.h"
#include "BaseGameEntity.h"
#include "Sheep.h"

//--------------------------- Instance ----------------------------------------
//
//   this class is a singleton
//-----------------------------------------------------------------------------
EntityManager* EntityManager::Instance()
{
	static EntityManager instance;

	return &instance;
}

//------------------------- GetEntityFromID -----------------------------------
//-----------------------------------------------------------------------------
BaseGameEntity* EntityManager::GetEntityFromID(int id)const
{
	//find the entity
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	//assert that the entity is a member of the map
	assert((ent != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

	return ent->second;
}

//--------------------------- RemoveEntity ------------------------------------
//-----------------------------------------------------------------------------
void EntityManager::RemoveEntity(BaseGameEntity* pEntity)
{
	m_EntityMap.erase(m_EntityMap.find(pEntity->ID()));
}

//---------------------------- RegisterEntity ---------------------------------
//-----------------------------------------------------------------------------
void EntityManager::RegisterEntity(BaseGameEntity* NewEntity)
{
	m_EntityMap.insert(std::make_pair(NewEntity->ID(), NewEntity));
}

MovingEntity* EntityManager::GetSheepBoss()
{
	return dynamic_cast <MovingEntity*> (m_EntityMap.find(0)->second);
}

bool EntityManager::IsSheepAllOutFromHouse()
{
	for (int i = 0; i < 5; i++)
	{
		auto sheepTemp = m_EntityMap.find(i);
		if (sheepTemp != m_EntityMap.end())
		{
			Sheep* sheep = dynamic_cast <Sheep*> (sheepTemp->second);
			if (!sheep->IsOutHouse())
			{
				return false;
			}
		}
	}

	return true;
}

bool EntityManager::IsHouseContainedAllSheep()
{
	for (int i = 0; i < 5; i++)
	{
		auto sheepTemp = m_EntityMap.find(i);
		if (sheepTemp != m_EntityMap.end())
		{
			Sheep* sheep = dynamic_cast <Sheep*> (sheepTemp->second);
			if (!sheep->IsContainedHouse())
			{
				return false;
			}
		}
	}

	return true;
}

bool EntityManager::IsSheepAllHungry()
{
	for (int i = 0; i < 5; i++)
	{
		auto sheepTemp = m_EntityMap.find(i);
		if (sheepTemp != m_EntityMap.end())
		{
			Sheep* sheep = dynamic_cast <Sheep*> (sheepTemp->second);
			if (!sheep->ishungry())
			{
				return false;
			}
		}
	}

	return true;
}

bool EntityManager::IsSheepAllFull()
{
	for (int i = 0; i < 5; i++)
	{
		auto sheepTemp = m_EntityMap.find(i);
		if (sheepTemp != m_EntityMap.end())
		{
			Sheep* sheep = dynamic_cast <Sheep*> (sheepTemp->second);
			if (!sheep->isFull())
			{
				return false;
			}
		}
	}

	return true;
}