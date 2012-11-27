#pragma once

#include "NetworkMessage.h"
#include "CreateCellErrorCode.h"

class CreateCellFailure : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	CreateCellErrorCode errorCode;

	/* Constructor / Destructor */
	CreateCellFailure();														/// Default constructor
	CreateCellFailure(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CreateCellFailure(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CreateCellFailure(const CreateCellFailure &other);								/// Copy constructor
	virtual ~CreateCellFailure();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};