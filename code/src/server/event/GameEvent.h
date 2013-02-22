#pragma once

#include "../game/IIdentifiable.h"

class GameEvent
{
protected:
	unsigned int id;
	double m_dDeadTime;
	bool terminated;
public:
	GameEvent() :
		terminated(false)
	{
		id = IIdentifiable::getNewId();
	}

	GameEvent(double startTime, double duration) :
	  m_dDeadTime(startTime + duration),
	  terminated(false)
	{
		id = IIdentifiable::getNewId();
	}

	unsigned int getId()
	{
		return id;
	}

	virtual void trigger() = 0;

	virtual double getDeadTime() const
	{
		return m_dDeadTime;
	}

	void setTerminated()
	{
		terminated = true;
	}
};