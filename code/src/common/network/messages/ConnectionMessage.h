#pragma once

#include "NetworkMessage.h"

#include <vector>

class ConnectionMessage : public NetworkMessage
{
public:
	/* Fields */
	std::vector<unsigned int> missingMessageIds;

	/* Constructor / Destructor */
	ConnectionMessage();												/// Default constructor
	ConnectionMessage(char *data, unsigned &index);						/// Construct from buffer starting by the given index
	ConnectionMessage(const NetworkMessage &other);						/// Copy constructor which copies the network address
	ConnectionMessage(const ConnectionMessage &other);					/// Copy constructor
	virtual ~ConnectionMessage();										/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};