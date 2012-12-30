#pragma once

#include "../../common/GameObject.h"
#include "IIdentifiable.h"

class GameObjectServer : 
	virtual public GameObject,
	public IIdentifiable
{
protected:
	GameObjectServer()
	{
		id = IIdentifiable::getNewId();
	}

	virtual void setId(unsigned int id) { throw logic_error("Not implemented exception"); }
};