#pragma once

#include "../../../../NetworkMessage.h"
#include "cinder/Vector.h"

class MovePolypeptideSuccess : public NetworkMessage
{
public:
	/* Fields */
	unsigned int requestId;
	std::vector<unsigned int> polypeptideIds;

	/* Constructor / Destructor */
	MovePolypeptideSuccess();														/// Default constructor
	MovePolypeptideSuccess(char *data, unsigned &index);							/// Construct from buffer starting by the given index
	MovePolypeptideSuccess(const NetworkMessage &other);							/// Copy constructor which copies the network address
	MovePolypeptideSuccess(const MovePolypeptideSuccess &other);								/// Copy constructor
	virtual ~MovePolypeptideSuccess();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};