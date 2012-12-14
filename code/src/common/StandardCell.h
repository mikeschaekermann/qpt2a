#pragma once

#include "Cell.h"

class StandardCell :
	virtual public Cell
{
public:
	static const float m_fAttackRadius;

	StandardCell(unsigned id) :
		GameObject(id),
		Cell(id)
	{
		m_fRadius = 5.f;
	}
};