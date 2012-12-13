#pragma once

class MessagingBehavior
{
public:
	MessagingBehavior(void);
	virtual ~MessagingBehavior(void) = 0;
	void virtual update(float frameTime) = 0;
};

