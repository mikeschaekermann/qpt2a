#pragma once

#include "../GameObject.h"

class StaticModificator : virtual public GameObject
{
public:
	enum Type
	{
		ANTIBIOTIC,
		NUTRIENTSOIL,
		RADIOACTIVITY
	};

	StaticModificator(unsigned int id, Vec3f position, Vec3f rotation, Vec3f scale, float radius, Type type)
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

	virtual void setPosition(Vec3f position) { throw logic_error("Not implemented exception"); }
	virtual void setRotation(Vec3f rotation) { throw logic_error("Not implemented exception"); }
	virtual void setScale(Vec3f scale) { throw logic_error("Not implemented exception"); }
	virtual void setRadius(float radius) { throw logic_error("Not implemented exception"); }
};