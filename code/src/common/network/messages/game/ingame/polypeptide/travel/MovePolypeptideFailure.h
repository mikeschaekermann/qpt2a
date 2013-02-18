#pragma once

#include "../../../../NetworkMessage.h"
#include "../../../../enum/MovePolypeptideErrorCode.h"

class MovePolypeptideFailure : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	MovePolypeptideErrorCode errorCode;

	/* Constructor / Destructor */
	MovePolypeptideFailure();														/// Default constructor
	MovePolypeptideFailure(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	MovePolypeptideFailure(const NetworkMessage &other);							/// Copy constructor which copies the network address
	MovePolypeptideFailure(const MovePolypeptideFailure &other);								/// Copy constructor
	virtual ~MovePolypeptideFailure();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};