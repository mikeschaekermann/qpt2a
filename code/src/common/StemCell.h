#pragma once

#include "ConfigurationDataHandler.h"
#include "Cell.h"

class StemCell :
	virtual public Cell
{
public:
	StemCell(Vec3f position, float angle) :
		Cell(position, CONFIG_FLOAT("data.cell.stemcell.radius"), angle, CONFIG_FLOAT("data.cell.stemcell.healthpoints"))
	{}
};
