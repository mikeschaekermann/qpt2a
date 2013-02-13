#pragma once

#include <vector>

#include "../../common/Cell.h"
#include "../../common/ConfigurationDataHandler.h"
#include "GameObjectServer.h"

class CellServer :
	virtual public Cell,
	virtual public GameObjectServer
{
public:
	enum Type
	{
		STEMCELL,
		STANDARDCELL,
		BONECELL
	};

	CellServer(Type type, Vec3f position, float angle, Player * owner) :
		Cell(position, angle, owner),
		type(type)
	{
		switch(type)
		{
			case STEMCELL:
				this->radius = CONFIG_FLOAT1("data.cell.stemcell.radius");
				this->healthPoints = CONFIG_FLOAT1("data.cell.stemcell.healthpoints");
				break;
			case STANDARDCELL:
				this->radius = CONFIG_FLOAT1("data.cell.standardcell.radius");
				this->healthPoints = CONFIG_FLOAT1("data.cell.standardcell.healthpoints");
				break;
			case BONECELL:
				this->radius = CONFIG_FLOAT1("data.cell.bonecell.radius");
				this->healthPoints = CONFIG_FLOAT1("data.cell.bonecell.healthpoints");
				break;
		}
	}

	void getNextCellPositionByAngle(float angle, float nextCellRadius, Vec3f & outPosition) const
	{
		outPosition = position;
		outPosition.x += cosf(angle) * (0.1f + nextCellRadius + this->radius);
		outPosition.y += sinf(angle) * (0.1f + nextCellRadius + this->radius);
	}

	Type getType() const { return type; }

private:
	virtual void setId(unsigned int id) { GameObjectServer::setId(id); }
	virtual void setPosition(Vec3f position) { Cell::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { Cell::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { Cell::setScale(scale); }
	virtual void setRadius(float radius) { Cell::setRadius(radius); }

	Type type;
};