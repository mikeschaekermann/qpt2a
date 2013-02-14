#pragma once

#include "../../common/Cell.h"
#include "../../client/actors/GameObjectClient.h"

class CellClient :
	virtual public Cell,
	virtual public GameObjectClient
{
public:
	virtual void setPosition(Vec3f position) { GameObject::setPosition(position); }
	virtual void drawHealthBar(float healthBarOpacity=1.0);

protected:
	virtual void setRotation(Vec3f rotation) { GameObject::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObject::setScale(scale); }
	virtual void setRadius(float radius) { GameObject::setRadius(radius); }
};