/**
* Tree Structure for Cells
* Every Cell is for itself a tree
*/

#pragma once

#ifndef M_PI
	#define M_PI 3.14159265359
#endif


#include <cmath>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "../common/Logger.h"

#include "IdGameObjectMap.h"
#include "PositionGameObjectMap.h"

using namespace std;

class GameObject;

class GameObjectContainer
{
public:
	void createGameObject(GameObject * gameObject)
	{
		if (idMap.addGameObject(gameObject))
		{
			if (positionMap.addGameObject(gameObject))
			{
				return;
			}
			LOG_INFO("Inserting gameobject in position map failed");
			idMap.removeGameObject(gameObject);
		}
		LOG_INFO("Inserting gameobject in id map failed");
	}

	void removeGameObject(unsigned int id)
	{
		GameObject * gameObject = find(id);
		if (gameObject != 0)
		{
			idMap.removeGameObject(gameObject);
			positionMap.removeGameObject(gameObject);
			delete gameObject;
		}
		else
		{
			LOG_INFO("Tried to remove a gameobject which does not exist");
		}
	}

	GameObject * find(unsigned int id) const
	{
		return idMap.find(id);
	}

	GameObject * find(Vec3f const & position) const
	{
		return positionMap.find(position);
	}

	const vector<GameObject *> findInRadiusOf(Vec3f const & position, float radius) const
	{
		return positionMap.findInRadiusOf(position, radius);
	}

	const vector<GameObject *> pick(const Vec3f & pickPosition) const
	{
		return positionMap.pick(pickPosition);
	}

	unsigned int getSize() const
	{
		if (idMap.getSize() != positionMap.getSize())
		{
			/// something wicked happens here
			LOG_ERROR("Problem with the maps");
		}
		return idMap.getSize();
	}

	map<unsigned int, GameObject*>::iterator begin()
	{
		return idMap.begin();
	}

	map<unsigned int, GameObject*>::iterator end()
	{
		return idMap.end();
	}
private:
	IdGameObjectMap idMap;
	PositionGameObjectMap positionMap;
};