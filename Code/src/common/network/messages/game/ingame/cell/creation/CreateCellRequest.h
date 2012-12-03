#pragma once

#include "../../../../NetworkMessage.h"
#include "../../../../enum/CellType.h"

class CreateCellRequest : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	unsigned int playerId;
	unsigned int cellId;
	float angle;
	CellType type;

	/* Constructor / Destructor */
	CreateCellRequest();														/// Default constructor
	CreateCellRequest(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CreateCellRequest(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CreateCellRequest(const CreateCellRequest &other);								/// Copy constructor
	virtual ~CreateCellRequest();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};