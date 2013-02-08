#pragma once

#include <map>

#include "GameObject.h"

using namespace std;

template<typename O>
class IGameObjectMap
{
public:
	virtual O * addGameObject(O * gameObject) = 0;
	virtual void removeGameObject(O * gameObject) = 0;
	virtual unsigned int getSize() const = 0;
};