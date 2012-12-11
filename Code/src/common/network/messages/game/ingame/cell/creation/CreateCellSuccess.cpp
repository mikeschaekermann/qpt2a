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

	memcpy(&position, &data[index], sizeof(position));
	index += sizeof(position);

	memcpy(&angle, &data[index], sizeof(angle));
	index += sizeof(angle);
}

CreateCellSuccess::CreateCellSuccess(const CreateCellSuccess &other) : NetworkMessage(other), requestId(other.requestId), cellId(other.cellId), angle(other.angle)
{
	messageType = MessageType::CreateCellSuccess;
	position[0] = other.position[0];
	position[1] = other.position[1];
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

	memcpy(&data[index], &position, sizeof(position));
	index += sizeof(position);

	memcpy(&data[index], &angle, sizeof(angle));
	index += sizeof(angle);
	
	return index;
}

unsigned CreateCellSuccess::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(cellId)
		+ sizeof(position)
		+ sizeof(angle);
}
