#pragma once

#include "../../common/Config.h"
#include "cinder/app/TouchEvent.h"
#include <vector>

using namespace ci;
using namespace app;

class TouchWay
{
public:
	/// @brief nested helper class representing one point within a touch way
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

	TouchWay();
	TouchWay(int32_t id, const Vec2f& startPos, double startTime);

	/// @brief gets the touch way's id
	int32_t getId() const;
	/// @brief gets the first position of the touch way
	Vec2f getStartPos() const;
	/// @brief gets the current position of the touch way
	Vec2f getCurrentPos() const;
	/**
		@brief adds a point to the touch way
		@param pos			position of the point in pixels
		@param time			time when this point within the touch way was triggered
	 */
	void addPoint(const Vec2f& pos, double time);
	/// @brief gets the vector of the last motion (i.e. the difference between current and previous touch point positions)
	Vec2f getLastDeltaVector() const;
	/// @brief gets a flag indicating whether this touch way is a simple click (i.e. no drag in between)
	bool isClick() const;

private:
	/// touch way's id
	const int32_t			m_iId;
	/// touch way consisting of a list of touch points
	vector<TouchPoint>		m_way;
};