#pragma once

#include "../game/GameObjectServer.h"
#include "../../common/environment/Barrier.h"

class BarrierServer : virtual public GameObjectServer, virtual public Barrier
{
public:
	BarrierServer(Vec3f position, Vec3f rotation, Vec3f scale, float radius) :
		GameObjectServer(),
		Barrier(IIdentifiable::getNewId(), position, rotation, scale, radius)
	{ }

private:
	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { Barrier::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Barrier::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Barrier::setScale(scale); }
	virtual void setRadius(float radius) { Barrier::setRadius(radius); }
};