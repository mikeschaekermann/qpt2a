#include "CreatePolypeptideSuccess.h"

CreatePolypeptideSuccess::CreatePolypeptideSuccess() : NetworkMessage(), requestId(0), polypeptideId(0)
{
	messageType = MessageType::CreatePolypeptideSuccess;
}

CreatePolypeptideSuccess::CreatePolypeptideSuccess(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), polypeptideId(0)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	memcpy(&polypeptideId, &data[index], sizeof(polypeptideId));
	polypeptideId = ntohl(polypeptideId);
	index += sizeof(polypeptideId);
}

CreatePolypeptideSuccess::CreatePolypeptideSuccess(const CreatePolypeptideSuccess &other) : NetworkMessage(other), requestId(other.requestId), polypeptideId(other.polypeptideId)
{
	messageType = MessageType::CreatePolypeptideSuccess;
}

CreatePolypeptideSuccess::CreatePolypeptideSuccess(const NetworkMessage &other) : NetworkMessage(other), requestId(0), polypeptideId(0)
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

	unsigned networkpolypeptideId = htonl(polypeptideId);
	memcpy(&data[index], &networkpolypeptideId, sizeof(networkpolypeptideId));
	index += sizeof(networkpolypeptideId);
	
	return index;
}

unsigned CreatePolypeptideSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(polypeptideId);
}
