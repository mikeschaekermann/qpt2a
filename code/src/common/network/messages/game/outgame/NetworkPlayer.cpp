#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif
#include <boost/asio.hpp>

#include "NetworkPlayer.h"
NetworkPlayer::NetworkPlayer()
{
	
}

NetworkPlayer::NetworkPlayer(char* data, unsigned &index)
{
	memcpy(&playerId, &data[index], sizeof(playerId));
	playerId = ntohl(playerId);
	index += sizeof(playerId);

	unsigned nameSize;
	memcpy(&nameSize, &data[index], sizeof(nameSize));
	nameSize = ntohl(nameSize);
	index += sizeof(nameSize);

	char *nameArray = new char[nameSize + 1];
	memcpy(nameArray, &data[index], nameSize);
	nameArray[nameSize] = 0;
	playerName = std::string(nameArray);
	index += nameSize;

	memcpy(&startCellId, &data[index], sizeof(startCellId));
	startCellId = ntohl(startCellId);
	index += sizeof(startCellId);

	memcpy(&startPosition.x, &data[index], sizeof(startPosition.x));
	index += sizeof(startPosition.x);

	memcpy(&startPosition.y, &data[index], sizeof(startPosition.y));
	index += sizeof(startPosition.y);

	memcpy(&startPosition.z, &data[index], sizeof(startPosition.z));
	index += sizeof(startPosition.z);
}

NetworkPlayer::NetworkPlayer(const NetworkPlayer &other) : playerId(other.playerId), playerName(other.playerName), startCellId(other.startCellId), startPosition(other.startPosition)
{
}

NetworkPlayer::~NetworkPlayer() 
{ 
}

unsigned NetworkPlayer::writeToArray(char* data, unsigned start)
{
	unsigned index = start;
	
	unsigned networkplayerId = htonl(playerId);
	memcpy(&data[index], &networkplayerId, sizeof(networkplayerId));
	index += sizeof(networkplayerId);

	unsigned networkNameSize = htonl(playerName.length());
	memcpy(&data[index], &networkNameSize, sizeof(networkNameSize));
	index += sizeof(networkNameSize);

	const char* nameArray = playerName.c_str();
	memcpy(&data[index], nameArray, playerName.length());
	index += playerName.length();

	unsigned networkstartCellId = htonl(startCellId);
	memcpy(&data[index], &networkstartCellId, sizeof(networkstartCellId));
	index += sizeof(networkstartCellId);

	memcpy(&data[index], &startPosition.x, sizeof(startPosition.x));
	index += sizeof(startPosition.x);
	
	memcpy(&data[index], &startPosition.y, sizeof(startPosition.y));
	index += sizeof(startPosition.y);

	memcpy(&data[index], &startPosition.z, sizeof(startPosition.z));
	index += sizeof(startPosition.z);

	return index;
}

unsigned NetworkPlayer::calculateSize()
{
	unsigned size = sizeof(playerId) +
		   sizeof(unsigned) +
		   playerName.size() +
		   sizeof(startCellId) +
		   3 * sizeof(float);
	

	return size;
}


