#pragma once

#include "../../../../NetworkMessage.h"

class PolypeptideCellAttack : public NetworkMessage
{
public:
	/* Fields */
	unsigned int attackerCellId;
	unsigned int attackedCellId;
	unsigned int polypeptideId;
	float damage;

	/* Constructor / Destructor */
	PolypeptideCellAttack();														/// Default constructor
	PolypeptideCellAttack(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	PolypeptideCellAttack(const NetworkMessage &other);							/// Copy constructor which copies the network address
	PolypeptideCellAttack(const PolypeptideCellAttack &other);								/// Copy constructor
	virtual ~PolypeptideCellAttack();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};