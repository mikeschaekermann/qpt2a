#include "StartGame.h"

StartGame::StartGame() : NetworkMessage(), worldRadius(0.f)
{
	messageType = MessageType::StartGame;
}

StartGame::StartGame(char* data, unsigned &index) : NetworkMessage(data, index), worldRadius(0.f)
{
	memcpy(&worldRadius, &data[index], sizeof(worldRadius));
	index += sizeof(worldRadius);

	unsigned playerCount;
	memcpy(&playerCount, &data[index], sizeof(playerCount));
	playerCount = ntohl(playerCount);
	index += sizeof(playerCount);

	for (unsigned i = 0; i < playerCount; ++i)
	{
		NetworkPlayer p(data, index);
		players.push_back(p);
	}
	
}

StartGame::StartGame(const NetworkMessage &other) : NetworkMessage(other), worldRadius(0.f)
{ 
	messageType = MessageType::StartGame;
}

StartGame::~StartGame() 
{ 
}

unsigned StartGame::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);

	memcpy(&data[index], &worldRadius, sizeof(worldRadius));
	index += sizeof(worldRadius);

	unsigned networkplayerCount = htonl(players.size());
	memcpy(&data[index], &networkplayerCount, sizeof(networkplayerCount));
	index += sizeof(networkplayerCount);

	for (unsigned i = 0; i < players.size(); ++i)
	{
		index = players[i].writeToArray(data, index);
	}
	
	return index;
}

unsigned StartGame::calculateSize()
{
	unsigned size = sizeof(worldRadius) +
		   sizeof(unsigned);

	for (unsigned i = 0; i < players.size(); ++i)
	{
		size += players[i].calculateSize();
	}

	return NetworkMessage::calculateSize() 
		+ size;
}