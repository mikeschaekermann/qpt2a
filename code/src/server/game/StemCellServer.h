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
};