#include "EnvironmentAttack.h"

EnvironmentAttack::EnvironmentAttack() : NetworkMessage(), modifierId(0), attackedCellId(0), damage(0.f)
{
	messageType = MessageType::EnvironmentAttack;
}

EnvironmentAttack::EnvironmentAttack(char* data, unsigned &index) : NetworkMessage(data, index), modifierId(0), attackedCellId(0), damage(0.f)
{
	memcpy(&modifierId, &data[index], sizeof(modifierId));
	modifierId = ntohl(modifierId);
	index += sizeof(modifierId);

	memcpy(&attackedCellId, &data[index], sizeof(attackedCellId));
	attackedCellId = ntohl(attackedCellId);
	index += sizeof(attackedCellId);

	memcpy(&damage, &data[index], sizeof(damage));
	index += sizeof(damage);
}

EnvironmentAttack::EnvironmentAttack(const NetworkMessage &other) : NetworkMessage(other), modifierId(0), attackedCellId(0), damage(0.f)
{
	messageType = MessageType::EnvironmentAttack;
}

EnvironmentAttack::~EnvironmentAttack() 
{ }

unsigned EnvironmentAttack::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkmodifierId = htonl(modifierId);
	memcpy(&data[index], &networkmodifierId, sizeof(networkmodifierId));
	index += sizeof(networkmodifierId);
	
	unsigned networkattackedCellId = htonl(attackedCellId);
	memcpy(&data[index], &networkattackedCellId, sizeof(networkattackedCellId));
	index += sizeof(networkattackedCellId);
	
	memcpy(&data[index], &damage, sizeof(damage));
	index += sizeof(damage);

	return index;
}

unsigned EnvironmentAttack::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(modifierId)
		+ sizeof(attackedCellId)
		+ sizeof(damage);
}
