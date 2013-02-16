#pragma once

#include <vector>

#include "../../common/Polypetide.h"
#include "../../common/ConfigurationDataHandler.h"

#include "GameObjectServer.h"

class PolypetideServer :
	virtual public Polypetide,
	virtual public GameObjectServer
{
public:
	PolypetideServer(Vec3f position, float angle, Player * owner) :
		Polypetide(position, angle, owner)
	{
		this->radius = CONFIG_FLOAT1("data.polypetide.radius");
		this->healthPoints = CONFIG_FLOAT1("data.polypetide.healthpoints");
	}

	void getNextCellPositionByAngle(float angle, float nextCellRadius, Vec3f & outPosition) const
	{
		outPosition = position;
		outPosition.x += cosf(angle) * (0.1f + nextCellRadius + this->radius);
		outPosition.y += sinf(angle) * (0.1f + nextCellRadius + this->radius);
	}

private:
	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { Polypetide::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Polypetide::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Polypetide::setScale(scale); }
	virtual void setRadius(float radius) { Polypetide::setRadius(radius); }
};