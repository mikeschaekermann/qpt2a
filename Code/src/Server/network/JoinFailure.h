#pragma once

#include "NetworkMessage.h"
#include "JoinErrorCode.h"

class JoinFailure : public NetworkMessage
{
public:
	/* Fields */
	JoinErrorCode errorCode;

	/* Constructor / Destructor */
	JoinFailure();														/// Default constructor
	JoinFailure(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	JoinFailure(const NetworkMessage &other);							/// Copy constructor which copies the network address
	JoinFailure(const JoinFailure &other);								/// Copy constructor
	virtual ~JoinFailure();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};