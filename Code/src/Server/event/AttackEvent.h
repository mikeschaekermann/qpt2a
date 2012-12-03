#pragma once

#include "Event.h"

#define ATTACK_EVENT_DURATION 30.f

class AttackEvent : public Event
{
public:
	AttackEvent() :
	  Event(ATTACK_EVENT_DURATION)
	  { }

	virtual void trigger()
	{

	}
};