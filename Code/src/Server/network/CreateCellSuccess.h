#pragma once

#include "NetworkMessage.h"

class CreateCellSuccess : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	unsigned int cellId;
	float position[2];
	float angle;

	/* Constructor / Destructor */
	CreateCellSuccess();														/// Default constructor
	CreateCellSuccess(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CreateCellSuccess(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CreateCellSuccess(const CreateCellSuccess &other);								/// Copy constructor
	virtual ~CreateCellSuccess();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};