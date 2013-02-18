#include "PolypeptideDie.h"

PolypeptideDie::PolypeptideDie() : NetworkMessage(), polypeptideId(0)
{
	messageType = MessageType::CellDie;
}

PolypeptideDie::PolypeptideDie(char* data, unsigned &index) : NetworkMessage(data, index), polypeptideId(0)
{
	memcpy(&polypeptideId, &data[index], sizeof(polypeptideId));
	polypeptideId = ntohl(polypeptideId);
	index += sizeof(polypeptideId);
}

PolypeptideDie::PolypeptideDie(const PolypeptideDie &other) : NetworkMessage(other), polypeptideId(other.polypeptideId)
{ 
	messageType = MessageType::CellDie;
}

PolypeptideDie::PolypeptideDie(const NetworkMessage &other) : NetworkMessage(other), polypeptideId(0)
{ 
	messageType = MessageType::CellDie;
}

PolypeptideDie::~PolypeptideDie() 
{ }

unsigned PolypeptideDie::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkPolypeptideId = htonl(polypeptideId);
	memcpy(&data[index], &networkPolypeptideId, sizeof(networkPolypeptideId));
	index += sizeof(networkPolypeptideId);
	
	return index;
}

unsigned PolypeptideDie::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(polypeptideId);
}