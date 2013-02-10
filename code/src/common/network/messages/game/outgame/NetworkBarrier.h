#pragma once

#include <vector>
#include "cinder/Vector.h"

class NetworkBarrier
{
public:
	/* Fields */
	unsigned modifierId;
	ci::Vec3f position;
	ci::Vec3f rotation;
	ci::Vec3f scale;
	float     radius;	

	/* Constructor / Destructor */
	NetworkBarrier();													/// Default constructor
	NetworkBarrier(char *data, unsigned &index);						/// Construct from buffer starting by the given index
	virtual ~NetworkBarrier();											/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};