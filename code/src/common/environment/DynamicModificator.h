#pragma once

#include "../GameObject.h"

class DynamicModificator : virtual public GameObject
{
public:
	enum Type
	{
		VIRISWARM,
		VITAMINGEYSER,
		LOOSEFATCELL
	};

	DynamicModificator(unsigned int id, Vec3f position, Vec3f rotation, Vec3f scale, float radius, Type type)
	{
		this->id = id;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->radius = radius;
		this->type = type;
	}

	Type getType() const { return type; }

protected:
	Type type;
};