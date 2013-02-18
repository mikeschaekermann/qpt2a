#pragma once

#include "../../../../NetworkMessage.h"
#include "../../../../enum/CellType.h"

class MovePolypeptideRequest : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	unsigned int fromCellId;
	unsigned int toCellId;
	unsigned int amount;

	/* Constructor / Destructor */
	MovePolypeptideRequest();														/// Default constructor
	MovePolypeptideRequest(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	MovePolypeptideRequest(const NetworkMessage &other);							/// Copy constructor which copies the network address
	MovePolypeptideRequest(const MovePolypeptideRequest &other);								/// Copy constructor
	virtual ~MovePolypeptideRequest();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};