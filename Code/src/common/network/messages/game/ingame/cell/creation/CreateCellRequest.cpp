#include "CreateCellRequest.h"

CreateCellRequest::CreateCellRequest() : NetworkMessage(), requestId(0), playerId(0), cellId(0), angle(0.f), type(CellType::Invalid)
{
	messageType = MessageType::CreateCellRequest;
}

CreateCellRequest::CreateCellRequest(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), playerId(0), cellId(0), angle(0.f), type(CellType::Invalid)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	memcpy(&playerId, &data[index], sizeof(playerId));
	playerId = ntohl(playerId);
	index += sizeof(playerId);

	memcpy(&cellId, &data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);

	memcpy(&angle, &data[index], sizeof(angle));
	index += sizeof(angle);

	unsigned networkCellType = 0;
	memcpy(&networkCellType, &data[index], sizeof(networkCellType));
	type = CellType(networkCellType);
	index += sizeof(networkCellType);
}

CreateCellRequest::CreateCellRequest(const CreateCellRequest &other) : NetworkMessage(other), 
	requestId(other.requestId), playerId(other.playerId), cellId(other.cellId), angle(other.angle), type(other.type)
{
	messageType = MessageType::CreateCellRequest;
}

CreateCellRequest::CreateCellRequest(const NetworkMessage &other) : NetworkMessage(other), requestId(0), playerId(0), cellId(0), angle(0.f), type(CellType::Invalid)
{ 
	messageType = MessageType::CreateCellRequest;
}

CreateCellRequest::~CreateCellRequest() 
{ }

unsigned CreateCellRequest::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned networkPlayerId = htonl(playerId);
	memcpy(&data[index], &networkPlayerId, sizeof(networkPlayerId));
	index += sizeof(networkPlayerId);

	unsigned networkcellId = htonl(cellId);
	memcpy(&data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);

	memcpy(&data[index], &angle, sizeof(angle));
	index += sizeof(angle);
	
	unsigned networkType = type.getNetworkType();
	memcpy(&data[index], &networkType, sizeof(networkType));
	index += sizeof(networkType);

	return index;
}

unsigned CreateCellRequest::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(playerId)
		+ sizeof(cellId)
		+ sizeof(angle)
		+ sizeof(type.getNetworkType());
}
