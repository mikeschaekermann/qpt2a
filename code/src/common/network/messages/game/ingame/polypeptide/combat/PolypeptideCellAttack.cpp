#include "PolypeptideCellAttack.h"

PolypeptideCellAttack::PolypeptideCellAttack() : NetworkMessage(), attackerCellId(0), attackedCellId(0), polypeptideId(0), damage(0.f)
{
	messageType = MessageType::PolypeptideCellAttack;
}

PolypeptideCellAttack::PolypeptideCellAttack(char* data, unsigned &index) : NetworkMessage(data, index), attackerCellId(0), attackedCellId(0), polypeptideId(0), damage(0.f)
{
	memcpy(&attackerCellId, &data[index], sizeof(attackerCellId));
	attackerCellId = ntohl(attackerCellId);
	index += sizeof(attackerCellId);

	memcpy(&attackedCellId, &data[index], sizeof(attackedCellId));
	attackedCellId = ntohl(attackedCellId);
	index += sizeof(attackedCellId);

	memcpy(&polypeptideId, &data[index], sizeof(polypeptideId));
	polypeptideId = ntohl(polypeptideId);
	index += sizeof(polypeptideId);

	memcpy(&damage, &data[index], sizeof(damage));
	index += sizeof(damage);
}

PolypeptideCellAttack::PolypeptideCellAttack(const PolypeptideCellAttack &other) : NetworkMessage(other), 
	attackerCellId(other.attackerCellId), attackedCellId(other.attackedCellId), polypeptideId(other.polypeptideId), damage(other.damage)
{ 
	messageType = MessageType::PolypeptideCellAttack;
}

PolypeptideCellAttack::PolypeptideCellAttack(const NetworkMessage &other) : NetworkMessage(other), attackerCellId(0), attackedCellId(0), polypeptideId(0), damage(0.f)
{ 
	messageType = MessageType::PolypeptideCellAttack;
}

PolypeptideCellAttack::~PolypeptideCellAttack() 
{ }

unsigned PolypeptideCellAttack::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);

	unsigned networkattackerCellId = htonl(attackerCellId);
	memcpy(&data[index], &networkattackerCellId, sizeof(networkattackerCellId));
	index += sizeof(networkattackerCellId);
	
	unsigned networkattackedCellId = htonl(attackedCellId);
	memcpy(&data[index], &networkattackedCellId, sizeof(networkattackedCellId));
	index += sizeof(networkattackedCellId);

	unsigned networkpolypeptideId = htonl(polypeptideId);
	memcpy(&data[index], &networkpolypeptideId, sizeof(networkpolypeptideId));
	index += sizeof(networkpolypeptideId);

	memcpy(&data[index], &damage, sizeof(damage));
	index += sizeof(damage);
	
	return index;
}

unsigned PolypeptideCellAttack::calculateSize()
{
	return NetworkMessage::calculateSize()
		+ sizeof(attackerCellId)
		+ sizeof(attackedCellId)
		+ sizeof(polypeptideId)
		+ sizeof(damage);
}