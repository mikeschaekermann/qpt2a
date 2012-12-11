#include "CellAttack.h"

CellAttack::CellAttack() : NetworkMessage(), attackerCellId(0), attackedCellId(0), damage(0.f)
{
	messageType = MessageType::CellAttack;
}

CellAttack::CellAttack(char* data, unsigned &index) : NetworkMessage(data, index), attackerCellId(0), attackedCellId(0), damage(0.f)
{
	memcpy(&attackerCellId, &data[index], sizeof(attackerCellId));
	attackerCellId = ntohl(attackerCellId);
	index += sizeof(attackerCellId);

	memcpy(&attackedCellId, &data[index], sizeof(attackedCellId));
	attackedCellId = ntohl(attackedCellId);
	index += sizeof(attackedCellId);

	memcpy(&damage, &data[index], sizeof(damage));
	index += sizeof(damage);
}

CellAttack::CellAttack(const CellAttack &other) : NetworkMessage(other), 
	attackerCellId(other.attackerCellId), attackedCellId(other.attackedCellId), damage(other.damage)
{ 
	messageType = MessageType::CellAttack;
}

CellAttack::CellAttack(const NetworkMessage &other) : NetworkMessage(other), attackerCellId(0), attackedCellId(0), damage(0.f)
{
	messageType = MessageType::CellAttack;
}

CellAttack::~CellAttack() 
{ }

unsigned CellAttack::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkattackerCellId = htonl(attackerCellId);
	memcpy(&data[index], &networkattackerCellId, sizeof(networkattackerCellId));
	index += sizeof(networkattackerCellId);
	
	unsigned networkattackedCellId = htonl(attackedCellId);
	memcpy(&data[index], &networkattackedCellId, sizeof(networkattackedCellId));
	index += sizeof(networkattackedCellId);
	
	memcpy(&data[index], &damage, sizeof(damage));
	index += sizeof(damage);

	return index;
}

unsigned CellAttack::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(attackerCellId)
		+ sizeof(attackedCellId)
		+ sizeof(damage);
}
