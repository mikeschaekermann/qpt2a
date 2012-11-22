#pragma once

#include "NetworkMessage.h"

class GameOver : public NetworkMessage
{
public:
	/* Fields */
	unsigned playerId;

	/* Constructor / Destructor */
	GameOver();														/// Default constructor
	GameOver(char *data, unsigned &index);								/// Construct from buffer starting by the given index
	GameOver(const NetworkMessage &other);								/// Copy constructor which copies the network address
	GameOver(const GameOver &other);									/// Copy constructor
	virtual ~GameOver();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};