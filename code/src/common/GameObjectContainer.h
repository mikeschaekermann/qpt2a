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
#include "CollisionHandler.h"
#include "ConfigurationDataHandler.h"

using namespace std;

class GameObject;

template<typename O=GameObject>
class GameObjectContainer
{
public:
	typedef map<unsigned int, typename O *> MapIdPointer;

	GameObjectContainer()
	{
		collisionHandler.initialize(ci::Area(0, 0, CONFIG_FLOAT1("data.world.radius") * 2, CONFIG_FLOAT1("data.world.radius") * 2));
	}

	void createGameObject(O * gameObject)
	{
		if (idMap.addGameObject(gameObject))
		{
			if (positionMap.addGameObject(gameObject))
			{
				collisionHandler.insert(Circle(gameObject->getId(), Vec2f(gameObject->getPosition().x, gameObject->getPosition().y), gameObject->getRadius()));
				return;
			}
			LOG_INFO("Inserting gameobject in position map failed");
			idMap.removeGameObject(gameObject);
		}
		LOG_INFO("Inserting gameobject in id map failed");
	}

	void removeGameObject(unsigned int id)
	{
		O * gameObject = find(id);
		if (gameObject != 0)
		{
			collisionHandler.remove(id);
			idMap.removeGameObject(gameObject);
			positionMap.removeGameObject(gameObject);
			delete gameObject;
		}
		else
		{
			LOG_INFO("Tried to remove a gameobject which does not exist");
		}
	}

	O * find(unsigned int id) const
	{
		return idMap.find(id);
	}

	O * find(Vec3f const & position) const
	{
		return positionMap.find(position);
	}

	const vector<O *> getGameObjectsToCheck(Vec3f const & position, float radius) const
	{
		// return positionMap.findInRadiusOf(position, radius);
		set<unsigned int> ids = collisionHandler.getCircleIndicesToCheck(Circle(0, Vec2f(position.x, position.y), radius));
		vector<O *> objects;
		for (auto it = ids.begin(); it != ids.end(); ++it)
		{
			objects.push_back(idMap.find(*it));
		}
		return objects;
	}

	const vector<O *> findInRadiusOf(Vec3f const & position, float radius) const
	{
		vector<O *> resObjects;
		const vector<O *> objects = getGameObjectsToCheck(position, radius);
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			if (isColliding((*it)->getPosition(), (*it)->getRadius(), position, radius))
			{
				resObjects.push_back(*it);
			}
		}

		return resObjects;
	}

	const vector<O *> pick(const Vec3f & pickPosition) const
	{
		//return positionMap.pick(pickPosition);
		vector<O *> resObjects;
		const vector<O *> objects = getGameObjectsToCheck(pickPosition, 0.f);
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			if (((*it)->getPosition() - pickPosition).length() < (*it)->getRadius())
			{
				resObjects.push_back(*it);
			}
		}

		return resObjects;
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

	typename MapIdPointer::iterator begin()
	{
		return idMap.begin();
	}

	typename MapIdPointer::iterator end()
	{
		return idMap.end();
	}
private:
	IdGameObjectMap<O> idMap;
	PositionGameObjectMap<O> positionMap;
	CollisionHandler collisionHandler;
};