#include "CellNew.h"

CellNew::CellNew() : NetworkMessage(), playerId(0), cellId(0), type(CellType::Invalid)
{
	messageType = MessageType::CellNew;
}

CellNew::CellNew(char* data, unsigned &index) : NetworkMessage(data, index), playerId(0), cellId(0), type(CellType::Invalid)
{
	memcpy(&playerId, &data[index], sizeof(playerId));
	playerId = ntohl(playerId);
	index += sizeof(playerId);

	memcpy(&cellId, &data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);

	memcpy(&position.x, &data[index], sizeof(position.x));
	index += sizeof(position.x);

	memcpy(&position.y, &data[index], sizeof(position.y));
	index += sizeof(position.y);

	memcpy(&position.z, &data[index], sizeof(position.z));
	index += sizeof(position.z);

	memcpy(&angle, &data[index], sizeof(angle));
	index += sizeof(angle);

	unsigned networkCellType = 0;
	memcpy(&networkCellType, &data[index], sizeof(networkCellType));
	type = CellType(networkCellType);
	index += sizeof(networkCellType);
}

CellNew::CellNew(const CellNew &other) : NetworkMessage(other), cellId(other.cellId), type(CellType::Invalid), position(other.position)
{
	messageType = MessageType::CellNew;
}

CellNew::CellNew(const NetworkMessage &other) : NetworkMessage(other), cellId(0), type(CellType::Invalid)
{ 
	messageType = MessageType::CellNew;
}

CellNew::~CellNew() 
{ }

unsigned CellNew::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkplayerId = htonl(playerId);
	memcpy(&data[index], &networkplayerId, sizeof(networkplayerId));
	index += sizeof(networkplayerId);

	unsigned networkcellId = htonl(cellId);
	memcpy(&data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);

	memcpy(&data[index], &position.x, sizeof(position.x));
	index += sizeof(position.x);
	
	memcpy(&data[index], &position.y, sizeof(position.y));
	index += sizeof(position.y);
	
	memcpy(&data[index], &position.z, sizeof(position.z));
	index += sizeof(position.z);

	memcpy(&data[index], &angle, sizeof(angle));
	index += sizeof(angle);

	unsigned networkType = type.getNetworkType();
	memcpy(&data[index], &networkType, sizeof(networkType));
	index += sizeof(networkType);
	
	return index;
}

unsigned CellNew::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(angle)
		+ sizeof(playerId)
		+ sizeof(cellId)
		+ sizeof(float) * 3
		+ sizeof(type.getNetworkType());
}