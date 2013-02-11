#pragma once

#include "../../../NetworkMessage.h"

class EnvironmentAttack : public NetworkMessage
{
public:
	/* Fields */
	unsigned int modifierId;
	unsigned int attackedCellId;
	float damage;

	/* Constructor / Destructor */
	EnvironmentAttack();											/// Default constructor
	EnvironmentAttack(char *data, unsigned &index);					/// Construct from buffer starting by the given index
	EnvironmentAttack(const NetworkMessage &other);					/// Copy constructor which copies the network address
	virtual ~EnvironmentAttack();									/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);	/// Serializes the class to the array
	virtual unsigned calculateSize();								/// Calculates and returns the size of this message
};