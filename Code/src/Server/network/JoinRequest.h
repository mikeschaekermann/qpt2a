#pragma once

#include "NetworkMessage.h"

class JoinRequest : public NetworkMessage
{
public:
	/* Fields */
	unsigned nameSize;
	const char* name;

	/* Constructor / Destructor */
	JoinRequest();														/// Default constructor
	JoinRequest(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	JoinRequest(const JoinRequest &other);								/// Copy constructor
	JoinRequest(const NetworkMessage &other);								/// Copy constructor which copies the network address
	virtual ~JoinRequest();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};