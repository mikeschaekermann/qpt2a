#pragma once

#include "../../common/Cell.h"
#include "../../client/actors/GameObjectClient.h"

class CellClient :
	virtual public Cell,
	virtual public GameObjectClient
{
protected:
	virtual void setPosition(Vec3f position) { Cell::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Cell::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Cell::setScale(scale); }
	virtual void setRadius(float radius) { Cell::setRadius(radius); }
};