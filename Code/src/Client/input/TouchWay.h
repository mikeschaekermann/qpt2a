#pragma once

#include "../../common/Config.h"
#include "cinder/app/TouchEvent.h"
#include <vector>

using namespace ci;
using namespace app;

class TouchWay
{
public:
	class TouchPoint
	{
	public:
		TouchPoint(const Vec2f &pos, double time): m_pos(pos), m_dTime(time) {}
		const Vec2f& getPos() const;
		double getTime() const;
	private:
		Vec2f	m_pos;
		double	m_dTime;
	};

	TouchWay(): m_iId(0) {};
	TouchWay(int32_t id, const Vec2f& startPos, double startTime);

	uint32_t getId() const { return m_iId; }
	Vec2f getStartPos() const { return m_way.begin()->getPos(); }
	void addPoint(const Vec2f& pos, double time) { m_way.push_back(TouchPoint(pos, time)); }
	Vec2f getLastDeltaVector() const
	{
		if (m_way.size() < 2)
		{
			return Vec2f(0, 0); 
		}
		else
		{
			return (m_way.back().getPos() - m_way[m_way.size() - 2].getPos());
		}
	}

private:
	const int32_t			m_iId;
	vector<TouchPoint>		m_way;
};