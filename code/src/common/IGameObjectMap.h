#pragma once

#include <map>

// usable types must be included here:
#include "GameObject.h"
#include "../client/actors/CellClient.h"
#include "../client/actors/GameObjectClient.h"

using namespace std;

template<typename O>
class IGameObjectMap
{
public:
	virtual O * addGameObject(O * gameObject) = 0;
	virtual void removeGameObject(O * gameObject) = 0;
	virtual unsigned int getSize() const = 0;
};