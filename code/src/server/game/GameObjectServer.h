#pragma once

#include "../../common/GameObject.h"
#include "IIdentifiable.h"

class GameObjectServer : 
	virtual public GameObject,
	public IIdentifiable
{
public:
	GameObjectServer() :
		GameObject(IIdentifiable::getNewId())
	{}
};