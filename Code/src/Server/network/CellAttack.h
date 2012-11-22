#pragma once

#include "NetworkMessage.h"

class CellAttack : public NetworkMessage
{
public:
	/* Fields */
	unsigned int attackerCellId;
	unsigned int attackedCellId;
	float damage;

	/* Constructor / Destructor */
	CellAttack();														/// Default constructor
	CellAttack(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CellAttack(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CellAttack(const CellAttack &other);								/// Copy constructor
	virtual ~CellAttack();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};