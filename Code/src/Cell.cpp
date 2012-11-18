#include "Cell.h"


Cell::Cell(Player* player, Vec3f position, Vec3f rotation, Vec3f scale):
	GameObject(player, position, rotation, scale),
	m_fHealthPoints(100)
{
}


Cell::~Cell(void)
{
}
