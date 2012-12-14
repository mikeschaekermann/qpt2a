#pragma once

#include <map>

#include "IGameObjectMap.h"

using namespace std;

class IdGameObjectMap : public IGameObjectMap
{
public:
	virtual GameObject* addGameObject(GameObject* gameObject)
	{
		//cell->setId(generateKey());
		pair<map<unsigned int, GameObject*>::iterator, bool> insertResult = m_map.insert(pair<unsigned int, GameObject*>(gameObject->getId(), gameObject));
		if (insertResult.second)
		{
			return insertResult.first->second;
		}
		return 0;
	}

	virtual void removeGameObject(GameObject* gameObject)
	{
		if (m_map.erase(gameObject->getId()))
		{
			/// worked
		}
	}

	GameObject* find(unsigned int id) const
	{
		map<unsigned int, GameObject*>::const_iterator it = m_map.find(id);
		if (it != m_map.end())
		{
			return it->second;
		}
		return 0;
	}

	virtual unsigned int getSize() const
	{
		return m_map.size();
	}
private:
	map<unsigned int, GameObject*> m_map;
	//static unsigned int m_uiMaxCellId;
	/*
	unsigned int generateKey() const
	{
		return ++m_uiMaxCellId;
	}
	*/
};
