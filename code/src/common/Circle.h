#pragma once

struct Circle
{
	Circle() {}
	Circle(unsigned int index, ci::Vec2f position, float radius) :
		index(index), position(position), radius(radius) { }
	bool operator<(const Circle & other) const
	{
		return this->index < other.index;
	}
	unsigned int index;
	ci::Vec2f position;
	float radius;
};