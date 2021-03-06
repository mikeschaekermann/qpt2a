#include "MovePolypeptideSuccess.h"

MovePolypeptideSuccess::MovePolypeptideSuccess() : NetworkMessage(), requestId(0), polypeptideIds(0)
{
	messageType = MessageType::MovePolypeptideSuccess;
}

MovePolypeptideSuccess::MovePolypeptideSuccess(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), polypeptideIds(0)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	unsigned int polypeptideIdCount;
	memcpy(&polypeptideIdCount, &data[index], sizeof(polypeptideIdCount));
	polypeptideIdCount = ntohl(polypeptideIdCount);
	index += sizeof(polypeptideIdCount);

	for (unsigned int i = 0; i < polypeptideIdCount; ++i)
	{
		unsigned int polypeptideId;
		memcpy(&polypeptideId, &data[index], sizeof(polypeptideId));
		polypeptideId = ntohl(polypeptideId);
		index += sizeof(polypeptideId);
		polypeptideIds.push_back(polypeptideId);
	}
}

MovePolypeptideSuccess::MovePolypeptideSuccess(const MovePolypeptideSuccess &other) : NetworkMessage(other), requestId(other.requestId), polypeptideIds(other.polypeptideIds)
{
	messageType = MessageType::MovePolypeptideSuccess;
}

MovePolypeptideSuccess::MovePolypeptideSuccess(const NetworkMessage &other) : NetworkMessage(other), requestId(0), polypeptideIds(0)
{ 
	messageType = MessageType::MovePolypeptideSuccess;
}

MovePolypeptideSuccess::~MovePolypeptideSuccess() 
{ }

unsigned MovePolypeptideSuccess::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned polypeptideIdsCount = htonl(polypeptideIds.size());
	memcpy(&data[index], &polypeptideIdsCount, sizeof(polypeptideIdsCount));
	index += sizeof(polypeptideIdsCount);

	for (auto it = polypeptideIds.begin(); it != polypeptideIds.end(); ++it)
	{
		unsigned int networkpolypeptideId = htonl(*it);
		memcpy(&data[index], &networkpolypeptideId, sizeof(networkpolypeptideId));
		index += sizeof(networkpolypeptideId);
	}
	
	return index;
}

unsigned MovePolypeptideSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(unsigned int)
		+ (sizeof(unsigned int) * polypeptideIds.size());
}
