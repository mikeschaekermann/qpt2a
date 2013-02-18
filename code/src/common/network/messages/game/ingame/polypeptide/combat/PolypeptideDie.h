#pragma once

#include "../../../../NetworkMessage.h"

class PolypeptideDie : public NetworkMessage
{
public:
	/* Fields */
	unsigned int polypeptideId;

	/* Constructor / Destructor */
	PolypeptideDie();														/// Default constructor
	PolypeptideDie(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	PolypeptideDie(const NetworkMessage &other);							/// Copy constructor which copies the network address
	PolypeptideDie(const PolypeptideDie &other);								/// Copy constructor
	virtual ~PolypeptideDie();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};