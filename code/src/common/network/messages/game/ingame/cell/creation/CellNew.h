#pragma once

#include "../../../../NetworkMessage.h"
#include "../../../../enum/CellType.h"

#include "cinder/Vector.h"

class CellNew : public NetworkMessage
{
public:
	/* Fields */
	unsigned int playerId;
	unsigned int cellId;
	ci::Vec3f position;
	float angle;
	CellType type;

	/* Constructor / Destructor */
	CellNew();														/// Default constructor
	CellNew(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	CellNew(const NetworkMessage &other);							/// Copy constructor which copies the network address
	CellNew(const CellNew &other);								/// Copy constructor
	virtual ~CellNew();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};