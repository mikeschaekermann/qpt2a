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

template<typename O=GameObject>
class GameObjectContainer
{
public:
	typedef map<unsigned int, typename O *> MapIdPointer;

	void createGameObject(O * gameObject)
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
		O * gameObject = find(id);
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

	O * find(unsigned int id) const
	{
		return idMap.find(id);
	}

	O * find(Vec3f const & position) const
	{
		return positionMap.find(position);
	}

	const vector<O *> findInRadiusOf(Vec3f const & position, float radius) const
	{
		return positionMap.findInRadiusOf(position, radius);
	}

	const vector<O *> pick(const Vec3f & pickPosition) const
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
};