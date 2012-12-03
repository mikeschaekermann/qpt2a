#include "CellNew.h"

CellNew::CellNew() : NetworkMessage(), playerId(0), cellId(0), type(CellType::Invalid)
{
	messageType = MessageType::CellNew;
}

CellNew::CellNew(char* data, unsigned &index) : NetworkMessage(data, index), playerId(0), cellId(0), type(CellType::Invalid)
{
	memcpy(&playerId, (void*) data[index], sizeof(playerId));
	playerId = ntohl(playerId);
	index += sizeof(playerId);

	memcpy(&cellId, (void*) data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);

	memcpy(&position, (void*) data[index], sizeof(position));
	index += sizeof(position);

	memcpy(&angle, (void*) data[index], sizeof(angle));
	index += sizeof(angle);

	unsigned networkCellType = 0;
	memcpy(&networkCellType, (void*) data[index], sizeof(networkCellType));
	type = CellType(networkCellType);
	index += sizeof(networkCellType);
}

CellNew::CellNew(const CellNew &other) : NetworkMessage(other), cellId(other.cellId), type(CellType::Invalid)
{
	messageType = MessageType::CellNew;
	position[0] = other.position[0];
	position[1] = other.position[1];
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
	memcpy((void*) data[index], &networkplayerId, sizeof(networkplayerId));
	index += sizeof(networkplayerId);

	unsigned networkcellId = htonl(cellId);
	memcpy((void*) data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);

	memcpy((void*) data[index], &position, sizeof(position));
	index += sizeof(position);

	unsigned networkType = type.getNetworkType();
	memcpy((void*) data[index], &networkType, sizeof(networkType));
	index += sizeof(networkType);
	
	return index;
}

unsigned CellNew::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(playerId)
		+ sizeof(cellId)
		+ sizeof(position)
		+ sizeof(type.getNetworkType());
}