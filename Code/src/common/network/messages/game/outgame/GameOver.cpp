#include "GameOver.h"

GameOver::GameOver() : NetworkMessage(), playerId(0)
{ 
	messageType = MessageType::GameOver;
}

GameOver::GameOver(char* data, unsigned &index) : NetworkMessage(data, index), playerId(0)
{
	memcpy(&playerId, &data[index], sizeof(playerId));
	playerId = ntohl(playerId);
	index += sizeof(playerId);
}

GameOver::GameOver(const GameOver &other) : NetworkMessage(other), playerId(other.playerId)
{
	messageType = MessageType::GameOver;
}

GameOver::GameOver(const NetworkMessage &other) : NetworkMessage(other), playerId(0)
{
	messageType = MessageType::GameOver;
}

GameOver::~GameOver() 
{ }

unsigned GameOver::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkPlayerId = htonl(playerId);
	memcpy(&data[index], &networkPlayerId, sizeof(networkPlayerId));
	index += sizeof(networkPlayerId);
	
	return index;
}

unsigned GameOver::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(playerId);
}