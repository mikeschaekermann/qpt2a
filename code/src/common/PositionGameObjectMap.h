#pragma once

#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "IGameObjectMap.h"

using namespace std;

class PositionGameObjectMap : public IGameObjectMap
{
public:
	virtual GameObject * addGameObject(GameObject * gameObject)
	{
		auto insertResult = gameObjectMap.insert(pair<float, map<float, GameObject *> >(gameObject->getPosition().x, map<float, GameObject *>()));
		auto insertResult2 = insertResult.first->second.insert(pair<float, GameObject *>(gameObject->getPosition().y, gameObject));
		
		if (insertResult2.second)
		{
			return insertResult2.first->second;
		}
		
		return 0;
	}

	virtual void removeGameObject(GameObject * gameObject)
	{
		map<float, map<float, GameObject *> >::iterator find = gameObjectMap.find(gameObject->getPosition()[0]);
		if (find != gameObjectMap.end())
		{
			if (find->second.erase(gameObject->getPosition()[1]))
			{
				/// worked
			}
		}
	}

	GameObject * find(const Vec3f & position) const
	{
		map<float, map<float, GameObject *> >::const_iterator gameObjectXIt = gameObjectMap.find(position.x);
		if (gameObjectXIt != gameObjectMap.end())
		{
			map<float, GameObject *>::const_iterator gameObjectYIt = gameObjectXIt->second.find(position.y);
			if (gameObjectYIt != gameObjectXIt->second.end())
			{
				return gameObjectYIt->second;
			}
		}
		return 0;
	}

	const vector<GameObject *> pick(const Vec3f & pickPosition) const
	{
		/// NAIVE PICKING IMPLEMENTATION
		/// IF THERE IS TIME AND THE PERFORMANCE NEED,
		/// QUADTREE-BASED PICKING COULD BE IMPLEMENTED!

		vector<GameObject *> gameObjectsPicked;

		for (auto xIt = gameObjectMap.begin(); xIt != gameObjectMap.end(); ++xIt)
		{
			for (auto yIt = xIt->second.begin(); yIt != xIt->second.end(); ++yIt)
			{
				auto distanceX = xIt->first - pickPosition.x;
				auto distanceY = yIt->first - pickPosition.y;
				auto gameObject = yIt->second;
				auto radius = gameObject->getRadius();

				if (distanceX * distanceX + distanceY * distanceY <= radius * radius)
				{
					gameObjectsPicked.push_back(gameObject);
				}
			}
		}

		return gameObjectsPicked;
	}

	const vector<GameObject *> findInRadiusOf(const Vec3f & position, float radius) const
	{
		vector<GameObject *> gameObjects;

		/// NEEDS TO BE REFACTORED!!!

		/*
		map<float, map<float, GameObject *> >::const_iterator startXIt = gameObjectMap.lower_bound(position.x);
		
		// lower_bound() retrurns container.end() if there is only one element below position.x, so ...
		if (startXIt == gameObjectMap.end() && gameObjectMap.size() > 0)
		{
			// in this case, also use upper_bound()
			startXIt = gameObjectMap.upper_bound(position.x);
		}

		if (startXIt != gameObjectMap.end())
		{
			map<float, map<float, GameObject *> >::const_iterator xIt = startXIt;
			while(true)
			{
				map<float, GameObject *>::const_iterator yIt = xIt->second.lower_bound(position.y);
				
				if (yIt == xIt->second.end())


				if (yIt != xIt->second.end())
				{
					for (; yIt != xIt->second.begin(); --yIt)
					{
						auto distanceX = xIt->first - position.x;
						auto distanceY = yIt->first - position.y;

						if (distanceX * distanceX + distanceY * distanceY <= radius * radius)
						{
							gameObjects.push_back(yIt->second);
						}
						else
						{
							break;
						}
					}
				}

				if (xIt == gameObjectMap.begin())
				{
					break;
				}
				else
				{
					--xIt;
				}
			}

			++startXIt;
			for (map<float, map<float, GameObject *> >::const_iterator xIt = startXIt; xIt != gameObjectMap.end(); ++xIt)
			{
				map<float, GameObject *>::const_iterator yIt = xIt->second.upper_bound(position.y);
				for (; yIt != xIt->second.end(); ++yIt)
				{
					if (sqrtf(xIt->first * xIt->first + yIt->first * yIt->first) <= radius)
					{
						gameObjects.push_back(yIt->second);
					}
					else
					{
						continue;
					}
				}
			}
		}
		*/
		return gameObjects;
	}

	virtual unsigned int getSize() const
	{
		unsigned int size = 0;
		map<float, map<float, GameObject *> >::const_iterator it = gameObjectMap.begin();
		for (; it != gameObjectMap.end(); ++it)
		{
			size += it->second.size();
		}
		return size;
	}
private:
	map<float, map<float, GameObject *> > gameObjectMap;
};