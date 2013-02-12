#include "AABBCollisionBehavior.h"

using namespace ci;

bool AABBCollisionBehavior::hasCollision(Circle const & c1, Circle const & c2) const
{
	return hasCollision(circleToArea(c1), circleToArea(c2));
}

bool AABBCollisionBehavior::hasCollision(Area const & a, Circle const & c) const
{
	return hasCollision(a, circleToArea(c));
}

bool AABBCollisionBehavior::hasCollision(Area const & a1, Area const & a2) const
{
	if (a1.x2 < a2.x1)
		return false;
	if (a1.x1 > a2.x2)
		return false;
	if (a1.y2 < a2.y1)
		return false;
	if (a1.y1 > a2.y2)
		return false;

	return true;
}

bool AABBCollisionBehavior::isCircleInArea(Area const & a, Circle const & c) const
{
	Area & ca = circleToArea(c);
	/*if (((a.x1 <= ca.x1 && ca.x1 <= a.x2) ||
		 (a.x1 <= ca.x2 && ca.x2 <= a.x2)) &&
		((a.y1 <= ca.y1 && ca.y1 <= a.y2) ||
		 (a.y1 <= ca.y2 && ca.y2 <= a.y2)))
	{
		return true;
	}

	return false;*/

	if (a.x2 < ca.x1)
		return false;
	if (a.x1 > ca.x2)
		return false;
	if (a.y2 < ca.y1)
		return false;
	if (a.y1 > ca.y2)
		return false;

	return true;
}

Area AABBCollisionBehavior::circleToArea(Circle const & c) const
{
	Area circleArea;
	circleArea.x1 = (int32_t) (c.position.x - c.radius);
	circleArea.y1 = (int32_t) (c.position.y - c.radius);
	circleArea.x2 = (int32_t) (c.position.x + c.radius);
	circleArea.y2 = (int32_t) (c.position.y + c.radius);

	return circleArea;
}