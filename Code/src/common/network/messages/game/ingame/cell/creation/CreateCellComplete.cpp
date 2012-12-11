#include "CreateCellComplete.h"

CreateCellComplete::CreateCellComplete() : NetworkMessage(), cellId(0)
{
	messageType = MessageType::CreateCellComplete;
}

CreateCellComplete::CreateCellComplete(char* data, unsigned &index) : NetworkMessage(data, index), cellId(0)
{
	memcpy(&cellId, &data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);
}

CreateCellComplete::CreateCellComplete(const CreateCellComplete &other) : NetworkMessage(other), cellId(other.cellId)
{ 
	messageType = MessageType::CreateCellComplete;
}

CreateCellComplete::CreateCellComplete(const NetworkMessage &other) : NetworkMessage(other), cellId(0)
{ 
	messageType = MessageType::CreateCellComplete;
}

CreateCellComplete::~CreateCellComplete() 
{ }

unsigned CreateCellComplete::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkcellId = htonl(cellId);
	memcpy(&data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);
	
	return index;
}

unsigned CreateCellComplete::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(cellId);
}