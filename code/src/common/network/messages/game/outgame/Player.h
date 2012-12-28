#pragma once

#include <vector>
#include "cinder/Vector.h"

class Player
{
public:
	/* Fields */
	unsigned playerId;
	std::string playerName;
	unsigned startCellId;
	ci::Vec3f startPosition;

	/* Constructor / Destructor */
	Player();														/// Default constructor
	Player(char *data, unsigned &index);								/// Construct from buffer starting by the given index
	Player(const Player &other);									/// Copy constructor
	virtual ~Player();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};