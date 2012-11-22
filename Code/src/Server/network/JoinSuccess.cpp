#include "JoinSuccess.h"

JoinSuccess::JoinSuccess() : NetworkMessage(), playerId(0)
{ }

JoinSuccess::JoinSuccess(char* data, unsigned &index) : NetworkMessage(data, index), playerId(0)
{
	memcpy(&playerId, (void*) data[index], sizeof(playerId));
	playerId = ntohl(playerId);
	index += sizeof(playerId);
}

JoinSuccess::JoinSuccess(const JoinSuccess &other) : NetworkMessage(other), playerId(other.playerId)
{ }

JoinSuccess::JoinSuccess(const NetworkMessage &other) : NetworkMessage(other), playerId(0)
{ }

JoinSuccess::~JoinSuccess() 
{ }

unsigned JoinSuccess::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkPlayerId = htonl(playerId);
	memcpy((void*) data[index], &networkPlayerId, sizeof(networkPlayerId));
	index += sizeof(networkPlayerId);
	
	return index;
}

unsigned JoinSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(playerId);
}