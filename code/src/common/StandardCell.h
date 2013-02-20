#pragma once

#include "ConfigurationDataHandler.h"
#include "Cell.h"

class StandardCell :
	virtual public Cell
{
public:
	static const float m_fAttackRadius;

	StandardCell(Vec3f position, float angle) :
		Cell(position, CONFIG_FLOAT("data.cell.standardcell.radius"), angle, CONFIG_FLOAT("data.cell.standardcell.healthpoints"))
	{ }
};