#pragma once

#include "../../../../NetworkMessage.h"

class PolypeptideFight : public NetworkMessage
{
public:
	/* Fields */
	unsigned int polypeptideId1;
	unsigned int polypeptideId2;
	bool polypeptide1Dies;
	bool polypeptide2Dies;

	/* Constructor / Destructor */
	PolypeptideFight();														/// Default constructor
	PolypeptideFight(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	PolypeptideFight(const NetworkMessage &other);							/// Copy constructor which copies the network address
	PolypeptideFight(const PolypeptideFight &other);								/// Copy constructor
	virtual ~PolypeptideFight();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};