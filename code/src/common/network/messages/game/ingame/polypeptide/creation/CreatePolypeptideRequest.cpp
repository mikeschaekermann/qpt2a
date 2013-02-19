#include "CreatePolypeptideRequest.h"

CreatePolypeptideRequest::CreatePolypeptideRequest() : NetworkMessage(), requestId(0), playerId(0)
{
	messageType = MessageType::CreatePolypeptideRequest;
}

CreatePolypeptideRequest::CreatePolypeptideRequest(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), playerId(0)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	memcpy(&playerId, &data[index], sizeof(playerId));
	playerId = ntohl(playerId);
	index += sizeof(playerId);
}

CreatePolypeptideRequest::CreatePolypeptideRequest(const CreatePolypeptideRequest &other) : NetworkMessage(other), 
	requestId(other.requestId), playerId(other.playerId)
{
	messageType = MessageType::CreatePolypeptideRequest;
}

CreatePolypeptideRequest::CreatePolypeptideRequest(const NetworkMessage &other) : NetworkMessage(other), requestId(0), playerId(0)
{ 
	messageType = MessageType::CreatePolypeptideRequest;
}

CreatePolypeptideRequest::~CreatePolypeptideRequest() 
{ }

unsigned CreatePolypeptideRequest::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned networkPlayerId = htonl(playerId);
	memcpy(&data[index], &networkPlayerId, sizeof(networkPlayerId));
	index += sizeof(networkPlayerId);

	return index;
}

unsigned CreatePolypeptideRequest::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(playerId);
}
