#pragma once

#include <vector>

#include "../../common/Cell.h"
#include "GameObjectServer.h"

class CellServer :
	virtual public Cell,
	virtual public GameObjectServer
{
public:
	enum Type
	{
		STEMCELL,
		STANDARDCELL
	};

	CellServer(Vec3f position, float radius, float angle, float healthPoints, Type type) :
		Cell(position, radius, angle, healthPoints),
		type(type)
	{ }

	void getNextCellPositionByAngle(float angle, float nextCellRadius, Vec3f & outPosition) const
	{
		outPosition = position;
		outPosition.x += cosf(ci::toRadians(angle)) * nextCellRadius;
		outPosition.y += sinf(ci::toRadians(angle)) * nextCellRadius;
	}

private:
	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { Cell::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Cell::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Cell::setScale(scale); }
	virtual void setRadius(float radius) { Cell::setRadius(radius); }

	Type type;
};