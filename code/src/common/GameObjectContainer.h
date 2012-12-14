/**
* Tree Structure for Cells
* Every Cell is for itself a tree
*/

#pragma once

#ifndef M_PI
	#define M_PI 3.14159265359
#endif

#include "../common/Config.h"

#include <cmath>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "IdGameObjectMap.h"
#include "PositionGameObjectMap.h"

using namespace std;

class GameObjectContainer
{
public:
	/// returns cell id: positive if valid
	int createGameObject(GameObject* gameObject, const Vec2f position, float angle)
	{
		if (m_idMap.addGameObject(gameObject))
		{
			if (m_positionMap.addGameObject(gameObject))
			{
				//gameObject->init(position, angle);
				return gameObject->getId();
			}
			/// error
		}
		/// error
		return -1;
	}

	void removeGameObject(unsigned int id)
	{
		GameObject* gameObject = find(id);
		if (gameObject != 0)
		{
			m_idMap.removeGameObject(gameObject);
			m_positionMap.removeGameObject(gameObject);
			delete gameObject;
		}
		else
		{
			/// error
		}
	}

	const Vec3f & getRelativePosition(unsigned int id, float angle) const
	{
		GameObject* gameObject = m_idMap.find(id);
		if (gameObject != 0)
		{
			auto position = gameObject->getPosition();

			position.x += cosf(angle * (float)M_PI / 180.f) * gameObject->getRadius();
			position.y += sinf(angle * (float)M_PI / 180.f) * gameObject->getRadius();
			
			return position;
		}
		return Vec3f();
	}

	GameObject* find(unsigned int id) const
	{
		return m_idMap.find(id);
	}

	GameObject* find(Vec3f const & position) const
	{
		return m_positionMap.find(position);
	}

	const vector<GameObject*> findInRadiusOf(Vec3f const & position, float radius) const
	{
		return m_positionMap.findInRadiusOf(position, radius);
	}

	unsigned int getSize() const
	{
		if (m_idMap.getSize() != m_positionMap.getSize())
		{
			/// something wicked happens here
		}
		return m_idMap.getSize();
	}
private:
	IdGameObjectMap m_idMap;
	PositionGameObjectMap m_positionMap;
};