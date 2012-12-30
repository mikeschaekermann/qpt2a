#pragma once

#include <vector>

#include "../../common/Cell.h"
#include "GameObjectServer.h"

class CellServer :
	virtual public Cell,
	virtual public GameObjectServer
{
public:
	void getNextCellPositionByAngle(float angle, float nextCellRadius, Vec3f & outPosition) const
	{
		outPosition = position;
		outPosition.x += cosf(angle * (float)M_PI / 180.f) * nextCellRadius;
		outPosition.y += sinf(angle * (float)M_PI / 180.f) * nextCellRadius;
	}
protected:
	CellServer(void) { }
	CellServer(Vec3f position, float radius, float angle, float healthPoints) :
		Cell(position, radius, angle, healthPoints)
	{ }

	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { Cell::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Cell::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Cell::setScale(scale); }
	virtual void setRadius(float radius) { Cell::setRadius(radius); }
};