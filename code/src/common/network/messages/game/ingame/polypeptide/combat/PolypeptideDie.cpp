#include "PolypeptideDie.h"

PolypeptideDie::PolypeptideDie() : NetworkMessage(), cellId(0), polypeptideId(0)
{
	messageType = MessageType::PolypeptideDie;
}

PolypeptideDie::PolypeptideDie(char* data, unsigned &index) : NetworkMessage(data, index), cellId(0), polypeptideId(0)
{
	memcpy(&cellId, &data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);

	memcpy(&polypeptideId, &data[index], sizeof(polypeptideId));
	polypeptideId = ntohl(polypeptideId);
	index += sizeof(polypeptideId);
}

PolypeptideDie::PolypeptideDie(const PolypeptideDie &other) : NetworkMessage(other), cellId(other.cellId), polypeptideId(other.polypeptideId)
{ 
	messageType = MessageType::PolypeptideDie;
}

PolypeptideDie::PolypeptideDie(const NetworkMessage &other) : NetworkMessage(other), cellId(0), polypeptideId(0)
{ 
	messageType = MessageType::PolypeptideDie;
}

PolypeptideDie::~PolypeptideDie() 
{ }

unsigned PolypeptideDie::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);

	unsigned networkcellId = htonl(cellId);
	memcpy(&data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);
	
	unsigned networkPolypeptideId = htonl(polypeptideId);
	memcpy(&data[index], &networkPolypeptideId, sizeof(networkPolypeptideId));
	index += sizeof(networkPolypeptideId);
	
	return index;
}

unsigned PolypeptideDie::calculateSize()
{
	return NetworkMessage::calculateSize()
		+ sizeof(cellId)
		+ sizeof(polypeptideId);
}