#include "CellDie.h"

CellDie::CellDie() : NetworkMessage(), cellId(0)
{
	messageType = MessageType::CellDie;
}

CellDie::CellDie(char* data, unsigned &index) : NetworkMessage(data, index), cellId(0)
{
	memcpy(&cellId, (void*) data[index], sizeof(cellId));
	cellId = ntohl(cellId);
	index += sizeof(cellId);
}

CellDie::CellDie(const CellDie &other) : NetworkMessage(other), cellId(other.cellId)
{ 
	messageType = MessageType::CellDie;
}

CellDie::CellDie(const NetworkMessage &other) : NetworkMessage(other), cellId(0)
{ 
	messageType = MessageType::CellDie;
}

CellDie::~CellDie() 
{ }

unsigned CellDie::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkcellId = htonl(cellId);
	memcpy((void*) data[index], &networkcellId, sizeof(networkcellId));
	index += sizeof(networkcellId);
	
	return index;
}

unsigned CellDie::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(cellId);
}