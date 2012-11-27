#pragma once

class Event
{
private:
	float m_fDuration;
protected:
	float m_fDeadTime;
public:
	Event(float duration) :
	  m_fDuration(duration)
	{ }

	bool operator<(Event& other)
	{
		return m_fDeadTime < other.m_fDeadTime;
	}

	virtual void trigger() = 0;

	void setDeadTime(float time)
	{
		m_fDeadTime = time + m_fDuration;
	}

	float getDeadTime() const
	{
		return m_fDeadTime;
	}
};