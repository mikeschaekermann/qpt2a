#include "PolypeptideFight.h"

PolypeptideFight::PolypeptideFight() : NetworkMessage(), polypeptideId1(0), polypeptideId2(0), polypeptide1Dies(0), polypeptide2Dies(0)
{
	messageType = MessageType::CellDie;
}

PolypeptideFight::PolypeptideFight(char* data, unsigned &index) : NetworkMessage(data, index), polypeptideId1(0), polypeptideId2(0), polypeptide1Dies(0), polypeptide2Dies(0)
{
	memcpy(&polypeptideId1, &data[index], sizeof(polypeptideId1));
	polypeptideId1 = ntohl(polypeptideId1);
	index += sizeof(polypeptideId1);

	memcpy(&polypeptideId2, &data[index], sizeof(polypeptideId2));
	polypeptideId2 = ntohl(polypeptideId2);
	index += sizeof(polypeptideId2);

	memcpy(&polypeptide1Dies, &data[index], sizeof(polypeptide1Dies));
	polypeptide1Dies = ntohl(polypeptide1Dies);
	index += sizeof(polypeptide1Dies);

	memcpy(&polypeptide2Dies, &data[index], sizeof(polypeptide2Dies));
	polypeptide2Dies = ntohl(polypeptide2Dies);
	index += sizeof(polypeptide2Dies);
}

PolypeptideFight::PolypeptideFight(const PolypeptideFight &other) : NetworkMessage(other), 
	polypeptideId1(other.polypeptideId1), polypeptideId2(other.polypeptideId2), polypeptide1Dies(other.polypeptide1Dies), polypeptide2Dies(other.polypeptide2Dies)
{ 
	messageType = MessageType::CellDie;
}

PolypeptideFight::PolypeptideFight(const NetworkMessage &other) : NetworkMessage(other), polypeptideId1(0), polypeptideId2(0), polypeptide1Dies(0), polypeptide2Dies(0)
{ 
	messageType = MessageType::CellDie;
}

PolypeptideFight::~PolypeptideFight() 
{ }

unsigned PolypeptideFight::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkpolypeptideId1 = htonl(polypeptideId1);
	memcpy(&data[index], &networkpolypeptideId1, sizeof(networkpolypeptideId1));
	index += sizeof(networkpolypeptideId1);

	unsigned networkpolypeptideId2 = htonl(polypeptideId2);
	memcpy(&data[index], &networkpolypeptideId2, sizeof(networkpolypeptideId2));
	index += sizeof(networkpolypeptideId2);

	unsigned networkpolypeptide1Dies = htonl(polypeptide1Dies);
	memcpy(&data[index], &networkpolypeptide1Dies, sizeof(networkpolypeptide1Dies));
	index += sizeof(networkpolypeptide1Dies);

	unsigned networkpolypeptide2Dies = htonl(polypeptide2Dies);
	memcpy(&data[index], &networkpolypeptide2Dies, sizeof(networkpolypeptide2Dies));
	index += sizeof(networkpolypeptide2Dies);
	
	return index;
}

unsigned PolypeptideFight::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(polypeptideId1)
		+ sizeof(polypeptideId2)
		+ sizeof(polypeptide1Dies)
		+ sizeof(polypeptide2Dies);
}