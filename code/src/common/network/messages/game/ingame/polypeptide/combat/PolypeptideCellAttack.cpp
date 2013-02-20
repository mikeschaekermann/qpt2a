#include "PolypeptideCellAttack.h"

PolypeptideCellAttack::PolypeptideCellAttack() : NetworkMessage(), polypeptideId(0), cellId(0), damage(0.f)
{
	messageType = MessageType::PolypeptideCellAttack;
}

PolypeptideCellAttack::PolypeptideCellAttack(char* data, unsigned &index) : NetworkMessage(data, index), polypeptideId(0), cellId(0), damage(0.f)
{
	memcpy(&polypeptideId, &data[index], sizeof(polypeptideId));
	polypeptideId = ntohl(polypeptideId);
	index += sizeof(polypeptideId);

	memcpy(&cellId, &data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);

	memcpy(&damage, &data[index], sizeof(damage));
	damage = ntohl(damage);
	index += sizeof(damage);
}

PolypeptideCellAttack::PolypeptideCellAttack(const PolypeptideCellAttack &other) : NetworkMessage(other), 
	polypeptideId(other.polypeptideId), cellId(other.cellId), damage(other.damage)
{ 
	messageType = MessageType::PolypeptideCellAttack;
}

PolypeptideCellAttack::PolypeptideCellAttack(const NetworkMessage &other) : NetworkMessage(other), polypeptideId(0), cellId(0), damage(0.f)
{ 
	messageType = MessageType::PolypeptideCellAttack;
}

PolypeptideCellAttack::~PolypeptideCellAttack() 
{ }

unsigned PolypeptideCellAttack::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkpolypeptideId = htonl(polypeptideId);
	memcpy(&data[index], &networkpolypeptideId, sizeof(networkpolypeptideId));
	index += sizeof(networkpolypeptideId);

	unsigned networkcellId = htonl(cellId);
	memcpy(&data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);

	unsigned networkdamage = htonl(damage);
	memcpy(&data[index], &networkdamage, sizeof(networkdamage));
	index += sizeof(networkdamage);
	
	return index;
}

unsigned PolypeptideCellAttack::calculateSize()
{
	return NetworkMessage::calculateSize()
		+ sizeof(polypeptideId)
		+ sizeof(cellId)
		+ sizeof(damage);
}