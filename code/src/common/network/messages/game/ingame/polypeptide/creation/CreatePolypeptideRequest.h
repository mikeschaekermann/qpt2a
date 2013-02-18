#pragma once

#include "../../../../NetworkMessage.h"
#include "../../../../enum/CellType.h"

class CreatePolypeptideRequest : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	unsigned int playerId;

	/* Constructor / Destructor */
	CreatePolypeptideRequest();														/// Default constructor
	CreatePolypeptideRequest(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CreatePolypeptideRequest(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CreatePolypeptideRequest(const CreatePolypeptideRequest &other);								/// Copy constructor
	virtual ~CreatePolypeptideRequest();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};