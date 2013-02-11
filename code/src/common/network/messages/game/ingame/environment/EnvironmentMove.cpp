#include "EnvironmentMove.h"

EnvironmentMove::EnvironmentMove() : NetworkMessage(), modifierId(0), moveToPosition(), travelTime(0.f)
{
	messageType = MessageType::EnvironmentMove;
}

EnvironmentMove::EnvironmentMove(char* data, unsigned &index) : NetworkMessage(data, index), modifierId(0), moveToPosition(), travelTime(0.f)
{
	memcpy(&modifierId, &data[index], sizeof(modifierId));
	modifierId = ntohl(modifierId);
	index += sizeof(modifierId);

	memcpy(&moveToPosition.x, &data[index], sizeof(moveToPosition.x));
	index += sizeof(moveToPosition.x);

	memcpy(&moveToPosition.y, &data[index], sizeof(moveToPosition.y));
	index += sizeof(moveToPosition.y);

	memcpy(&moveToPosition.z, &data[index], sizeof(moveToPosition.z));
	index += sizeof(moveToPosition.z);

	memcpy(&travelTime, &data[index], sizeof(travelTime));
	index += sizeof(travelTime);
}

EnvironmentMove::EnvironmentMove(const NetworkMessage &other) : NetworkMessage(other), modifierId(0), moveToPosition(), travelTime(0.f)
{
	messageType = MessageType::EnvironmentMove;
}

EnvironmentMove::~EnvironmentMove() 
{ }

unsigned EnvironmentMove::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkmodifierId = htonl(modifierId);
	memcpy(&data[index], &networkmodifierId, sizeof(networkmodifierId));
	index += sizeof(networkmodifierId);
	
	memcpy(&data[index], &moveToPosition.x, sizeof(moveToPosition.x));
	index += sizeof(moveToPosition.x);
	
	memcpy(&data[index], &moveToPosition.y, sizeof(moveToPosition.y));
	index += sizeof(moveToPosition.y);

	memcpy(&data[index], &moveToPosition.z, sizeof(moveToPosition.z));
	index += sizeof(moveToPosition.z);
	
	memcpy(&data[index], &travelTime, sizeof(travelTime));
	index += sizeof(travelTime);

	return index;
}

unsigned EnvironmentMove::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(modifierId)
		+ sizeof(float) * 3
		+ sizeof(travelTime);
}
