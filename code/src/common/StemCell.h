#pragma once

#include "ConfigurationDataHandler.h"
#include "Cell.h"

class StemCell :
	virtual public Cell
{
public:
	StemCell(Vec3f position, float angle) :
		Cell(position, CONFIG_FLOAT1("cell.stemcell.radius"), angle, CONFIG_FLOAT1("cell.stemcell.healthpoints"))
	{ }
};
