#pragma once

#include "IGameObjectMap.h"

using namespace std;

template<typename O>
class IdGameObjectMap : public IGameObjectMap<typename O>
{
public:	
	typedef map<unsigned int, typename O *> MapIdPointer;

	virtual O * addGameObject(O * gameObject)
	{
		auto insertResult = gameObjectMap.insert(pair<unsigned int, O *>(gameObject->getId(), gameObject));
		if (insertResult.second)
		{
			return insertResult.first->second;
		}
		return 0;
	}

	virtual void removeGameObject(O * gameObject)
	{
		if (gameObjectMap.erase(gameObject->getId()))
		{
			/// worked
		}
	}

	O * find(unsigned int id) const
	{
		map<unsigned int, O *>::const_iterator it = gameObjectMap.find(id);
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

	virtual typename MapIdPointer::iterator begin()
	{
		return gameObjectMap.begin();
	}

	virtual typename MapIdPointer::iterator end()
	{
		return gameObjectMap.end();
	}
private:
	MapIdPointer gameObjectMap;
};
