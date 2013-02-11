#pragma once

#include "../game/GameObjectServer.h"
#include "../../common/environment/DynamicModificator.h"

class DynamicModificatorServer : virtual public GameObjectServer, virtual public DynamicModificator
{
public:
	DynamicModificatorServer(Vec3f position, Vec3f rotation, Vec3f scale, float radius, DynamicModificator::Type type) :
		GameObjectServer(),
		DynamicModificator(IIdentifiable::getNewId(), position, rotation, scale, radius, type)
	{ }

private:
	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { DynamicModificator::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { DynamicModificator::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { DynamicModificator::setScale(scale); }
	virtual void setRadius(float radius) { DynamicModificator::setRadius(radius); }
};