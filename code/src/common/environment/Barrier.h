#pragma once

#include "../GameObject.h"

class Barrier : virtual public GameObject
{
public:
	Barrier(unsigned int id, Vec3f position, Vec3f rotation, Vec3f scale, float radius)
	{
		this->id = id;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->radius = radius;
	}

protected:
	virtual void setPosition(Vec3f position) { throw logic_error("Not implemented exception"); }
	virtual void setRotation(Vec3f rotation) { throw logic_error("Not implemented exception"); }
	virtual void setScale(Vec3f scale) { throw logic_error("Not implemented exception"); }
	virtual void setRadius(float radius) { throw logic_error("Not implemented exception"); }
};