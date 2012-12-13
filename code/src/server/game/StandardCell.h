#pragma once

#include "Cell.h"

class StandardCell : public Cell
{
public:
	static const float m_fAttackRadius;
	StandardCell() : Cell(5.f) { }
};