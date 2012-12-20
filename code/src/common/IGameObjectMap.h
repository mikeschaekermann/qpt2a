#pragma once

#include <map>

#include "GameObject.h"

using namespace std;

class IGameObjectMap
{
public:
	virtual GameObject * addGameObject(GameObject * gameObject) = 0;
	virtual void removeGameObject(GameObject * gameObject) = 0;
	virtual unsigned int getSize() const = 0;
};