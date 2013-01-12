#pragma once

#include "IGameObjectMap.h"

using namespace std;

class IdGameObjectMap : public IGameObjectMap
{
public:	
	virtual GameObject * addGameObject(GameObject * gameObject)
	{
		auto insertResult = gameObjectMap.insert(pair<unsigned int, GameObject *>(gameObject->getId(), gameObject));
		if (insertResult.second)
		{
			return insertResult.first->second;
		}
		return 0;
	}

	virtual void removeGameObject(GameObject * gameObject)
	{
		if (gameObjectMap.erase(gameObject->getId()))
		{
			/// worked
		}
	}

	GameObject * find(unsigned int id) const
	{
		map<unsigned int, GameObject *>::const_iterator it = gameObjectMap.find(id);
		if (it != gameObjectMap.end())
		{
			return it->second;
		}
		return 0;
	}

	virtual unsigned int getSize() const
	{
		return gameObjectMap.size();
	}
private:
	map<unsigned int, GameObject *> gameObjectMap;
};
