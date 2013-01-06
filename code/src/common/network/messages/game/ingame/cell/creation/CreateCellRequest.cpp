#include "CreateCellRequest.h"

CreateCellRequest::CreateCellRequest() : NetworkMessage(), requestId(0), playerId(0), cellId(0), type(CellType::Invalid)
{
	messageType = MessageType::CreateCellRequest;
}

CreateCellRequest::CreateCellRequest(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), playerId(0), cellId(0), type(CellType::Invalid)
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
	
	memcpy(&rotation.x, &data[index], sizeof(rotation.x));
	index += sizeof(rotation.x);

	memcpy(&rotation.y, &data[index], sizeof(rotation.y));
	index += sizeof(rotation.y);

	memcpy(&rotation.z, &data[index], sizeof(rotation.z));
	index += sizeof(rotation.z);

	unsigned networkCellType = 0;
	memcpy(&networkCellType, &data[index], sizeof(networkCellType));
	type = CellType(networkCellType);
	index += sizeof(networkCellType);
}

CreateCellRequest::CreateCellRequest(const CreateCellRequest &other) : NetworkMessage(other), 
	requestId(other.requestId), playerId(other.playerId), cellId(other.cellId), rotation(other.rotation), type(other.type)
{
	messageType = MessageType::CreateCellRequest;
}

CreateCellRequest::CreateCellRequest(const NetworkMessage &other) : NetworkMessage(other), requestId(0), playerId(0), cellId(0), type(CellType::Invalid)
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

	memcpy(&data[index], &rotation.x, sizeof(rotation.x));
	index += sizeof(rotation.x);
	
	memcpy(&data[index], &rotation.y, sizeof(rotation.y));
	index += sizeof(rotation.y);
	
	memcpy(&data[index], &rotation.z, sizeof(rotation.z));
	index += sizeof(rotation.z);
	
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
		+ sizeof(float) * 3
		+ sizeof(type.getNetworkType());
}
