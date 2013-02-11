#include "ResourceBonus.h"

ResourceBonus::ResourceBonus() : NetworkMessage(), modifierId(0), cellId(0), amount(0.f), duration(0.f)
{
	messageType = MessageType::ResourceBonus;
}

ResourceBonus::ResourceBonus(char* data, unsigned &index) : NetworkMessage(data, index), modifierId(0), cellId(0), amount(0.f), duration(0.f)
{
	memcpy(&modifierId, &data[index], sizeof(modifierId));
	modifierId = ntohl(modifierId);
	index += sizeof(modifierId);

	memcpy(&cellId, &data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);

	memcpy(&amount, &data[index], sizeof(amount));
	index += sizeof(amount);
	 
	memcpy(&duration, &data[index], sizeof(duration));
	index += sizeof(duration);
}

ResourceBonus::ResourceBonus(const NetworkMessage &other) : NetworkMessage(other), modifierId(0), cellId(0), amount(0.f), duration(0.f)
{
	messageType = MessageType::ResourceBonus;
}

ResourceBonus::~ResourceBonus() 
{ }

unsigned ResourceBonus::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkmodifierId = htonl(modifierId);
	memcpy(&data[index], &networkmodifierId, sizeof(networkmodifierId));
	index += sizeof(networkmodifierId);

	unsigned networkcellId = htonl(cellId);
	memcpy(&data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);

	memcpy(&data[index], &amount, sizeof(amount));
	index += sizeof(amount);

	memcpy(&data[index], &duration, sizeof(duration));
	index += sizeof(duration);

	return index;
}

unsigned ResourceBonus::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(modifierId)
		+ sizeof(cellId)
		+ sizeof(amount)
		+ sizeof(duration);
}
