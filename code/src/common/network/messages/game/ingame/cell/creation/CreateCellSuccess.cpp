#include "CreateCellSuccess.h"

CreateCellSuccess::CreateCellSuccess() : NetworkMessage(), requestId(0), cellId(0), angle(0.f)
{
	messageType = MessageType::CreateCellSuccess;
}

CreateCellSuccess::CreateCellSuccess(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), cellId(0), angle(0.f)
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

	memcpy(&angle, &data[index], sizeof(angle));
	index += sizeof(angle);
}

CreateCellSuccess::CreateCellSuccess(const CreateCellSuccess &other) : NetworkMessage(other), requestId(other.requestId), cellId(other.cellId), angle(other.angle)
{
	messageType = MessageType::CreateCellSuccess;
}

CreateCellSuccess::CreateCellSuccess(const NetworkMessage &other) : NetworkMessage(other), requestId(0), cellId(0), angle(0.f)
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

	memcpy(&data[index], &angle, sizeof(angle));
	index += sizeof(angle);
	
	return index;
}

unsigned CreateCellSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(cellId)
		+ sizeof(float) * 3
		+ sizeof(angle);
}
