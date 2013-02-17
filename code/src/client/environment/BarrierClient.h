#pragma once

#include "../actors/GameObjectClient.h"
#include "../../common/environment/Barrier.h"

class BarrierClient : virtual public GameObjectClient, virtual public Barrier
{
public:
	BarrierClient(unsigned id, Vec3f position, Vec3f rotation, Vec3f scale, float radius) :
		GameObjectClient(),
		Barrier(id, position, rotation, scale, radius)
	{ opacity = 1.f; }
protected:
	void drawAtTransformation() const;
private:
	virtual void setId(unsigned int id) { GameObjectClient::setId(id); }
	virtual void setPosition(Vec3f position) { Barrier::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Barrier::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Barrier::setScale(scale); }
	virtual void setRadius(float radius) { Barrier::setRadius(radius); }
};