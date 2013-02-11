#pragma once

#include "../../../NetworkMessage.h"

class ResourceBonus : public NetworkMessage
{
public:
	/* Fields */
	unsigned int modifierId;
	unsigned int cellId;
	float amount;
	float duration;

	/* Constructor / Destructor */
	ResourceBonus();												/// Default constructor
	ResourceBonus(char *data, unsigned &index);						/// Construct from buffer starting by the given index
	ResourceBonus(const NetworkMessage &other);						/// Copy constructor which copies the network address
	virtual ~ResourceBonus();										/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);	/// Serializes the class to the array
	virtual unsigned calculateSize();								/// Calculates and returns the size of this message
};