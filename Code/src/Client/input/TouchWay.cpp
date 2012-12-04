#include "TouchWay.h"

const Vec2f& TouchWay::TouchPoint::getPos() const { return m_pos; }
double TouchWay::TouchPoint::getTime() const { return m_dTime; }

TouchWay::TouchWay(int32_t id, const Vec2f& startPos, double startTime):
	m_way(),
	m_iId(id)
{
	m_way.push_back(TouchPoint(startPos, startTime));
}