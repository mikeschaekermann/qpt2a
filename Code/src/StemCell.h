#pragma once
#include "Cell.h"
class StemCell :
	public Cell
{
public:
	StemCell(unsigned id, Player* player, Vec3f position = Vec3f(0, 0, 0), Vec3f rotation = Vec3f(0, 0, 0), Vec3f scale = Vec3f(1, 1, 1));
	~StemCell(void);
};

