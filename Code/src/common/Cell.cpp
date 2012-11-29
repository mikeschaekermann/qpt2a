#include "Cell.h"


Cell::Cell(unsigned id, Player* player, Vec3f position, Vec3f rotation, Vec3f scale):
	GameObject(id, position, rotation, scale),
	m_pOwner(player),
	m_fHealthPoints(100)
{
}


Cell::~Cell(void)
{
}
