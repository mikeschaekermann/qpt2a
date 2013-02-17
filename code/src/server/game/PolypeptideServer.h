#pragma once

#include <vector>

#include "../../common/Polypeptide.h"
#include "../../common/ConfigurationDataHandler.h"

#include "GameObjectServer.h"

class PolypeptideServer :
	virtual public Polypeptide,
	virtual public GameObjectServer
{
public:
	PolypeptideServer(Vec3f position, float angle, Player * owner) :
		Polypeptide(position, angle, owner)
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
	virtual void setPosition(Vec3f position) { Polypeptide::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Polypeptide::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Polypeptide::setScale(scale); }
	virtual void setRadius(float radius) { Polypeptide::setRadius(radius); }
};