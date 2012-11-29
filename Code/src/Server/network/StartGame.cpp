#include "StartGame.h"

StartGame::StartGame() : NetworkMessage(), worldRadius(0.f), playerCount(0), playerIds(0), playerNameSizes(0), playerNames(0)
{
	messageType = MessageType::StartGame;
}

StartGame::StartGame(char* data, unsigned &index) : NetworkMessage(data, index), worldRadius(0.f), playerCount(0), playerIds(0), playerNameSizes(0), playerNames(0)
{
	memcpy(&worldRadius, (void*) data[index], sizeof(worldRadius));
	index += sizeof(worldRadius);


	memcpy(&playerCount, (void*) data[index], sizeof(playerCount));
	playerCount = ntohl(playerCount);
	index += sizeof(playerCount);

	playerIds = new unsigned[playerCount];
	playerNameSizes = new unsigned[playerCount];
	playerNames = new char*[playerCount];
	startCellIds = new unsigned[playerCount];
	startPositions = new float*[playerCount];

	for (unsigned i = 0; i < playerCount; ++i)
	{
		memcpy(&playerIds[i], (void*) data[index], sizeof(playerIds[i]));
		playerIds[i] = ntohl(playerIds[i]);
		index += sizeof(playerIds[i]);

		memcpy(&playerNameSizes[i], (void*) data[index], sizeof(playerNameSizes[i]));
		playerNameSizes[i] = ntohl(playerNameSizes[i]);
		index += sizeof(playerNameSizes[i]);

		playerNames[i] = new char[playerNameSizes[i]];
		memcpy((void*) playerNames[i], (void*) data[index], playerNameSizes[i]);
		index += playerNameSizes[i];

		memcpy(&startCellIds[i], (void*) data[index], sizeof(startCellIds[i]));
		startCellIds[i] = ntohl(startCellIds[i]);
		index += sizeof(startCellIds[i]);

		startPositions[i] = new float[2];
		memcpy((void*) playerNames[i], (void*) data[index], 2 * sizeof(float));
		index += (2 * sizeof(float));
	}
}

StartGame::StartGame(const StartGame &other) : NetworkMessage(other), worldRadius(other.worldRadius), playerCount(other.playerCount)
{ 
	messageType = MessageType::StartGame;

	playerIds = new unsigned[playerCount];
	playerNameSizes = new unsigned[playerCount];
	playerNames = new char*[playerCount];
	startCellIds = new unsigned[playerCount];
	startPositions = new float*[playerCount];

	for (unsigned i = 0; i < playerCount; ++i)
	{
		playerIds[i] = other.playerIds[i];
		playerNameSizes[i] = other.playerNameSizes[i];
		startCellIds[i] = other.startCellIds[i];
		
		playerNames[i] = new char[playerNameSizes[i]];
		memcpy((void*) playerNames[i], (void*) other.playerNames[i], playerNameSizes[i]);

		startPositions[i] = new float[2];
		memcpy((void*) startPositions[i], (void*) other.startPositions[i], 2 * sizeof(float));
	}
}

StartGame::StartGame(const NetworkMessage &other) : NetworkMessage(other), worldRadius(0.f), playerCount(0), playerIds(0), playerNameSizes(0), playerNames(0)
{ 
	messageType = MessageType::StartGame;
}

StartGame::~StartGame() 
{ 
	if (playerIds)
	{
		delete[] playerIds;
	}

	if (playerNameSizes)
	{
		delete[] playerNameSizes;
	}

	if (startCellIds)
	{
		delete[] startCellIds;
	}

	for (unsigned i = 0; i < playerCount; ++i)
	{
		delete[] playerNames[i];
		delete[] startPositions[i];
	}
}

unsigned StartGame::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);

	memcpy((void*) data[index], &worldRadius, sizeof(worldRadius));
	index += sizeof(worldRadius);

	unsigned networkplayerCount = htonl(playerCount);
	memcpy((void*) data[index], &networkplayerCount, sizeof(networkplayerCount));
	index += sizeof(networkplayerCount);

	for (unsigned i = 0; i < playerCount; ++i)
	{
		unsigned networkplayerId = htonl(playerIds[i]);
		memcpy((void*) data[index], &networkplayerId, sizeof(networkplayerId));
		index += sizeof(networkplayerId);

		unsigned networkplayerNameSizes = htonl(playerNameSizes[i]);
		memcpy((void*) data[index], &networkplayerNameSizes, sizeof(networkplayerNameSizes));
		index += sizeof(networkplayerNameSizes);

		memcpy((void*) data[index], (void*) playerNames[i], sizeof(playerNameSizes[i]));
		index += playerNameSizes[i];

		unsigned networkstartCellId = htonl(startCellIds[i]);
		memcpy((void*) data[index], &networkstartCellId, sizeof(networkstartCellId));
		index += sizeof(networkstartCellId);

		memcpy((void*) data[index], (void*) startPositions[i], 2 * sizeof(float));
		index += (2 * sizeof(float));
	}
	
	return index;
}

unsigned StartGame::calculateSize()
{
	unsigned size = sizeof(worldRadius) +
		   sizeof(playerCount) +
		   playerCount * sizeof(playerIds) +
		   playerCount * sizeof(startCellIds) + 
		   playerCount * 2 * sizeof(float);

	for (unsigned i = 0; i < playerCount; ++i)
	{
		size += playerNameSizes[i];
	}

	return NetworkMessage::calculateSize() 
		+ size;
}