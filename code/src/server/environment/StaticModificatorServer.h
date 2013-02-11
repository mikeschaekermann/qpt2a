#pragma once

#include "../game/GameObjectServer.h"
#include "../../common/environment/StaticModificator.h"

class StaticModificatorServer : virtual public GameObjectServer, virtual public StaticModificator
{
public:
	StaticModificatorServer(Vec3f position, Vec3f rotation, Vec3f scale, float radius, StaticModificator::Type type) :
		GameObjectServer(),
		StaticModificator(IIdentifiable::getNewId(), position, rotation, scale, radius, type)
	{ }

private:
	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { StaticModificator::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { StaticModificator::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { StaticModificator::setScale(scale); }
	virtual void setRadius(float radius) { StaticModificator::setRadius(radius); }
};