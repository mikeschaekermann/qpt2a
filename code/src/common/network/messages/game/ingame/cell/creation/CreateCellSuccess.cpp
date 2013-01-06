#include "CreateCellSuccess.h"

CreateCellSuccess::CreateCellSuccess() : NetworkMessage(), requestId(0), cellId(0)
{
	messageType = MessageType::CreateCellSuccess;
}

CreateCellSuccess::CreateCellSuccess(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), cellId(0)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	memcpy(&cellId, &data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);

	memcpy(&position.x, &data[index], sizeof(position.x));
	index += sizeof(position.x);

	memcpy(&position.y, &data[index], sizeof(position.y));
	index += sizeof(position.y);

	memcpy(&position.z, &data[index], sizeof(position.z));
	index += sizeof(position.z);
	
	memcpy(&rotation.x, &data[index], sizeof(rotation.x));
	index += sizeof(rotation.x);

	memcpy(&rotation.y, &data[index], sizeof(rotation.y));
	index += sizeof(rotation.y);

	memcpy(&rotation.z, &data[index], sizeof(rotation.z));
	index += sizeof(rotation.z);
}

CreateCellSuccess::CreateCellSuccess(const CreateCellSuccess &other) : NetworkMessage(other), requestId(other.requestId), cellId(other.cellId), rotation(other.rotation)
{
	messageType = MessageType::CreateCellSuccess;
}

CreateCellSuccess::CreateCellSuccess(const NetworkMessage &other) : NetworkMessage(other), requestId(0), cellId(0)
{ 
	messageType = MessageType::CreateCellSuccess;
}

CreateCellSuccess::~CreateCellSuccess() 
{ }

unsigned CreateCellSuccess::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned networkcellId = htonl(cellId);
	memcpy(&data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);

	memcpy(&data[index], &position.x, sizeof(position.x));
	index += sizeof(position.x);
	
	memcpy(&data[index], &position.y, sizeof(position.y));
	index += sizeof(position.y);
	
	memcpy(&data[index], &position.z, sizeof(position.z));
	index += sizeof(position.z);
	
	memcpy(&data[index], &rotation.x, sizeof(rotation.x));
	index += sizeof(rotation.x);
	
	memcpy(&data[index], &rotation.y, sizeof(rotation.y));
	index += sizeof(rotation.y);
	
	memcpy(&data[index], &rotation.z, sizeof(rotation.z));
	index += sizeof(rotation.z);
	
	return index;
}

unsigned CreateCellSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(cellId)
		+ sizeof(float) * 3
		+ sizeof(float) * 3;
}
