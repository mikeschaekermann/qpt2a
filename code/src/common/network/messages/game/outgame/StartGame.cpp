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

	unsigned barrierCount;
	memcpy(&barrierCount, &data[index], sizeof(barrierCount));
	barrierCount = ntohl(barrierCount);
	index += sizeof(barrierCount);

	for (unsigned i = 0; i < barrierCount; ++i)
	{
		NetworkBarrier p(data, index);
		barriers.push_back(p);
	}

	unsigned staticCount;
	memcpy(&staticCount, &data[index], sizeof(staticCount));
	staticCount = ntohl(staticCount);
	index += sizeof(staticCount);

	for (unsigned i = 0; i < staticCount; ++i)
	{
		NetworkStaticModifier p(data, index);
		staticModifiers.push_back(p);
	}

	unsigned dynamicCount;
	memcpy(&dynamicCount, &data[index], sizeof(dynamicCount));
	dynamicCount = ntohl(dynamicCount);
	index += sizeof(dynamicCount);

	for (unsigned i = 0; i < dynamicCount; ++i)
	{
		NetworkDynamicModifier p(data, index);
		dynamicModifiers.push_back(p);
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

	unsigned networkbarrierCount = htonl(barriers.size());
	memcpy(&data[index], &networkbarrierCount, sizeof(networkbarrierCount));
	index += sizeof(networkbarrierCount);

	for (unsigned i = 0; i < barriers.size(); ++i)
	{
		index = barriers[i].writeToArray(data, index);
	}

	unsigned networkstaticCount = htonl(staticModifiers.size());
	memcpy(&data[index], &networkstaticCount, sizeof(networkstaticCount));
	index += sizeof(networkstaticCount);

	for (unsigned i = 0; i < staticModifiers.size(); ++i)
	{
		index = staticModifiers[i].writeToArray(data, index);
	}

	unsigned networkdynamicCount = htonl(dynamicModifiers.size());
	memcpy(&data[index], &networkdynamicCount, sizeof(networkdynamicCount));
	index += sizeof(networkdynamicCount);

	for (unsigned i = 0; i < dynamicModifiers.size(); ++i)
	{
		index = dynamicModifiers[i].writeToArray(data, index);
	}
	
	return index;
}

unsigned StartGame::calculateSize()
{
	unsigned size = sizeof(worldRadius) +
		   sizeof(unsigned) +
		   sizeof(unsigned) +
		   sizeof(unsigned) +
		   sizeof(unsigned);

	for (unsigned i = 0; i < players.size(); ++i)
	{
		size += players[i].calculateSize();
	}

	for (unsigned i = 0; i < barriers.size(); ++i)
	{
		size += barriers[i].calculateSize();
	}

	for (unsigned i = 0; i < staticModifiers.size(); ++i)
	{
		size += staticModifiers[i].calculateSize();
	}

	for (unsigned i = 0; i < dynamicModifiers.size(); ++i)
	{
		size += dynamicModifiers[i].calculateSize();
	}

	return NetworkMessage::calculateSize() 
		+ size;
}