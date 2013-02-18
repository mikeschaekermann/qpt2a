#pragma once

#include "../../../../NetworkMessage.h"
#include "cinder/Vector.h"

class CreatePolypeptideSuccess : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;

	/* Constructor / Destructor */
	CreatePolypeptideSuccess();														/// Default constructor
	CreatePolypeptideSuccess(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CreatePolypeptideSuccess(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CreatePolypeptideSuccess(const CreatePolypeptideSuccess &other);								/// Copy constructor
	virtual ~CreatePolypeptideSuccess();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};