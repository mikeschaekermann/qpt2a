#pragma once

#include "NetworkMessage.h"

class JoinSuccess : public NetworkMessage
{
public:
	/* Fields */
	unsigned playerId;

	/* Constructor / Destructor */
	JoinSuccess();														/// Default constructor
	JoinSuccess(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	JoinSuccess(const NetworkMessage &other);							/// Copy constructor which copies the network address
	JoinSuccess(const JoinSuccess &other);								/// Copy constructor
	virtual ~JoinSuccess();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};