#pragma once

#include "../../../../NetworkMessage.h"
#include "../../../../enum/CreatePolypeptideErrorCode.h"

class CreatePolypeptideFailure : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	CreatePolypeptideErrorCode errorCode;

	/* Constructor / Destructor */
	CreatePolypeptideFailure();														/// Default constructor
	CreatePolypeptideFailure(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CreatePolypeptideFailure(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CreatePolypeptideFailure(const CreatePolypeptideFailure &other);								/// Copy constructor
	virtual ~CreatePolypeptideFailure();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};