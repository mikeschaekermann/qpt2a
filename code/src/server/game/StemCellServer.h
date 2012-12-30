#pragma once

#include "../../common/StemCell.h"
#include "CellServer.h"

class StemCellServer :
	virtual public StemCell,
	virtual public CellServer
{
public:
	StemCellServer(Vec3f position, float angle) :
		StemCell(position, angle)
	{ }
	
private:
	virtual void setId(unsigned int id) { CellServer::setId(id); }
	virtual void setPosition(Vec3f position) { CellServer::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { CellServer::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { CellServer::setScale(scale); }
	virtual void setRadius(float radius) { CellServer::setRadius(radius); }
};