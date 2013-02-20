#include "TouchWay.h"
#include "../../common/ConfigurationDataHandler.h"

const Vec2f& TouchWay::TouchPoint::getPos() const { return m_pos; }
double TouchWay::TouchPoint::getTime() const { return m_dTime; }

TouchWay::TouchWay(): m_iId(0), m_way() {};

TouchWay::TouchWay(int32_t id, const Vec2f& startPos, double startTime, TRIGGER trigger):
	m_way(),
	m_iId(id),
	trigger(trigger),
	wayLength(0)
{
	m_way.push_back(TouchPoint(startPos, startTime));
}

int32_t TouchWay::getId() const
{
	return m_iId;
}

Vec2f TouchWay::getStartPos() const
{
	return m_way.begin()->getPos();
}

Vec2f TouchWay::getCurrentPos() const
{
	return m_way.back().getPos();
}

void TouchWay::addPoint(const Vec2f& pos, double time)
{
	m_way.push_back(TouchPoint(pos, time));
	wayLength += getLastDeltaVector().length();
}

Vec2f TouchWay::getLastDeltaVector() const
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

float TouchWay::getLength() const
{
	return wayLength;
}

bool TouchWay::isClick() const
{
	auto distance = getStartPos().distance(getCurrentPos());
	auto clickPixelTolerance = CONFIG_FLOAT("data.input.clickPixelTolerance");
	auto clickWayLengthTolerance = CONFIG_FLOAT("data.input.clickWayLengthTolerance");

	return (distance <= clickPixelTolerance && wayLength <= clickWayLengthTolerance);
}

TouchWay::TRIGGER TouchWay::getTrigger() const
{
	return trigger;
}