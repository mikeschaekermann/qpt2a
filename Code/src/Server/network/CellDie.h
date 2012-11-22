#pragma once

#include "NetworkMessage.h"

class CellDie : public NetworkMessage
{
public:
	/* Fields */
	unsigned int cellId;

	/* Constructor / Destructor */
	CellDie();														/// Default constructor
	CellDie(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CellDie(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CellDie(const CellDie &other);								/// Copy constructor
	virtual ~CellDie();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};