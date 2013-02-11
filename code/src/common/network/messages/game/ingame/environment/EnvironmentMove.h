#pragma once

#include "cinder/Vector.h"
#include "../../../NetworkMessage.h"

class EnvironmentMove : public NetworkMessage
{
public:
	/* Fields */
	unsigned int modifierId;
	ci::Vec3f moveToPosition;
	float travelTime;

	/* Constructor / Destructor */
	EnvironmentMove();												/// Default constructor
	EnvironmentMove(char *data, unsigned &index);					/// Construct from buffer starting by the given index
	EnvironmentMove(const NetworkMessage &other);					/// Copy constructor which copies the network address
	virtual ~EnvironmentMove();										/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);	/// Serializes the class to the array
	virtual unsigned calculateSize();								/// Calculates and returns the size of this message
};