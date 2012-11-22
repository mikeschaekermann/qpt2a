#pragma once

#include "NetworkMessage.h"

class CreateCellComplete : public NetworkMessage
{
public:
	/* Fields */
	unsigned int cellId;

	/* Constructor / Destructor */
	CreateCellComplete();														/// Default constructor
	CreateCellComplete(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CreateCellComplete(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CreateCellComplete(const CreateCellComplete &other);								/// Copy constructor
	virtual ~CreateCellComplete();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};