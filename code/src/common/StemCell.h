#pragma once

#include "Cell.h"

class StemCell :
	virtual public Cell
{
public:
	StemCell(unsigned id) :
		GameObject(id),
		Cell(id)
	{
		m_fRadius = 10.f;
	}
};
