#pragma once

#include "../../common/StandardCell.h"
#include "CellServer.h"

class StandardCellServer :
	virtual public StandardCell,
	virtual public CellServer
{
public:
	StandardCellServer(Vec3f position, float angle) :
		GameObject(),
		Cell(position, CONFIG_FLOAT1("data.cell.standardcell.radius"), angle, CONFIG_FLOAT1("data.cell.standardcell.healthpoints")),
		GameObjectServer(),
		CellServer(),
		StandardCell(position, angle)
	{ }

private:
	virtual void setId(unsigned int id) { CellServer::setId(id); }
	virtual void setPosition(Vec3f position) { CellServer::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { CellServer::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { CellServer::setScale(scale); }
	virtual void setRadius(float radius) { CellServer::setRadius(radius); }
};