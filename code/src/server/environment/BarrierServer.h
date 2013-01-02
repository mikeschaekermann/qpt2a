#pragma once

#include "../game/GameObjectServer.h"
#include "../../common/environment/Barrier.h"

class BarrierServer : public GameObjectServer, public Barrier
{
public:
	BarrierServer(Vec3f position, Vec3f rotation, Vec3f scale, float radius) :
		GameObjectServer(),
		Barrier(position, rotation, scale, radius)
	{ }

private:
	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { BarrierServer::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { BarrierServer::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { BarrierServer::setScale(scale); }
	virtual void setRadius(float radius) { BarrierServer::setRadius(radius); }
};