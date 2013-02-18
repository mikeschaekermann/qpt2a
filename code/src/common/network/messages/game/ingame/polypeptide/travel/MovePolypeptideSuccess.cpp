#include "MovePolypeptideSuccess.h"

MovePolypeptideSuccess::MovePolypeptideSuccess() : NetworkMessage(), requestId(0), polypeptideIds(0)
{
	messageType = MessageType::CreateCellSuccess;
}

MovePolypeptideSuccess::MovePolypeptideSuccess(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), polypeptideIds(0)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	/// polypeptideIds is missing
}

MovePolypeptideSuccess::MovePolypeptideSuccess(const MovePolypeptideSuccess &other) : NetworkMessage(other), requestId(other.requestId), polypeptideIds(other.polypeptideIds)
{
	messageType = MessageType::CreateCellSuccess;
}

MovePolypeptideSuccess::MovePolypeptideSuccess(const NetworkMessage &other) : NetworkMessage(other), requestId(0), polypeptideIds(0)
{ 
	messageType = MessageType::CreateCellSuccess;
}

MovePolypeptideSuccess::~MovePolypeptideSuccess() 
{ }

unsigned MovePolypeptideSuccess::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	/// polypeptideIds is missing
	
	return index;
}

unsigned MovePolypeptideSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(polypeptideIds);
}
