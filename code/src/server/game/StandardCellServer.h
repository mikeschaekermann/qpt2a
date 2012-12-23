#pragma once

#include "../../common/StandardCell.h"
#include "CellServer.h"

class StandardCellServer :
	virtual public StandardCell,
	virtual public CellServer
{
public:
	StandardCellServer(Vec3f position, float angle) :
		StandardCell(position, angle)
	{ }
};