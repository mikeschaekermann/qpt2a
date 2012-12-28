#pragma once

#include <vector>
#include "cinder/Vector.h"

class NetworkPlayer
{
public:
	/* Fields */
	unsigned playerId;
	std::string playerName;
	unsigned startCellId;
	ci::Vec3f startPosition;

	/* Constructor / Destructor */
	NetworkPlayer();														/// Default constructor
	NetworkPlayer(char *data, unsigned &index);								/// Construct from buffer starting by the given index
	NetworkPlayer(const NetworkPlayer &other);									/// Copy constructor
	virtual ~NetworkPlayer();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};