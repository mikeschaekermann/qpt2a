#pragma once

#include "Event.h"

#define BUILDING_EVENT_DURATION 30.f

class BuildingEvent : public Event
{
public:
	BuildingEvent() :
	  Event(BUILDING_EVENT_DURATION)
	  { }

	virtual void trigger()
	{

	}
};