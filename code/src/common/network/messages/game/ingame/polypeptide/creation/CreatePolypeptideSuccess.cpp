#include "CreatePolypeptideSuccess.h"

CreatePolypeptideSuccess::CreatePolypeptideSuccess() : NetworkMessage(), requestId(0)
{
	messageType = MessageType::CreatePolypeptideSuccess;
}

CreatePolypeptideSuccess::CreatePolypeptideSuccess(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);
}

CreatePolypeptideSuccess::CreatePolypeptideSuccess(const CreatePolypeptideSuccess &other) : NetworkMessage(other), requestId(other.requestId)
{
	messageType = MessageType::CreatePolypeptideSuccess;
}

CreatePolypeptideSuccess::CreatePolypeptideSuccess(const NetworkMessage &other) : NetworkMessage(other), requestId(0)
{ 
	messageType = MessageType::CreatePolypeptideSuccess;
}

CreatePolypeptideSuccess::~CreatePolypeptideSuccess() 
{ }

unsigned CreatePolypeptideSuccess::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);
	
	return index;
}

unsigned CreatePolypeptideSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId);
}
