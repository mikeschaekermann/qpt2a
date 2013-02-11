#pragma once

#include <cinder/Area.h>
#include "Circle.h"

class AABBCollisionBehavior
{
public:
	bool hasCollision(Circle const & c1, Circle const & c2) const;
	bool hasCollision(ci::Area const & a, Circle const & c) const;
	bool hasCollision(ci::Area const & a1, ci::Area const & a2) const;
	bool isCircleInArea(ci::Area const & a, Circle const & c) const;
private:
	ci::Area circleToArea(Circle const & c) const;
};