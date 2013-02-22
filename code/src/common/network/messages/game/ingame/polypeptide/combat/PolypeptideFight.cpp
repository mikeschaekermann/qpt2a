#include "PolypeptideFight.h"

PolypeptideFight::PolypeptideFight() : NetworkMessage(), cell1Id(0), cell2Id(0), polypeptideId1(0), polypeptideId2(0), polypeptide1Dies(0), polypeptide2Dies(0)
{
	messageType = MessageType::PolypeptideFight;
}

PolypeptideFight::PolypeptideFight(char* data, unsigned &index) : NetworkMessage(data, index), cell1Id(0), cell2Id(0), polypeptideId1(0), polypeptideId2(0), polypeptide1Dies(0), polypeptide2Dies(0)
{
	memcpy(&cell1Id, &data[index], sizeof(cell1Id));
	cell1Id = ntohl(cell1Id);
	index += sizeof(cell1Id);

	memcpy(&cell2Id, &data[index], sizeof(cell2Id));
	cell2Id = ntohl(cell2Id);
	index += sizeof(cell2Id);

	memcpy(&polypeptideId1, &data[index], sizeof(polypeptideId1));
	polypeptideId1 = ntohl(polypeptideId1);
	index += sizeof(polypeptideId1);

	memcpy(&polypeptideId2, &data[index], sizeof(polypeptideId2));
	polypeptideId2 = ntohl(polypeptideId2);
	index += sizeof(polypeptideId2);

	memcpy(&polypeptide1Dies, &data[index], sizeof(polypeptide1Dies));
	index += sizeof(polypeptide1Dies);

	memcpy(&polypeptide2Dies, &data[index], sizeof(polypeptide2Dies));
	index += sizeof(polypeptide2Dies);
}

PolypeptideFight::PolypeptideFight(const PolypeptideFight &other) : NetworkMessage(other), 
	cell1Id(other.cell1Id), cell2Id(other.cell1Id), polypeptideId1(other.polypeptideId1), polypeptideId2(other.polypeptideId2), polypeptide1Dies(other.polypeptide1Dies), polypeptide2Dies(other.polypeptide2Dies)
{ 
	messageType = MessageType::PolypeptideFight;
}

PolypeptideFight::PolypeptideFight(const NetworkMessage &other) : NetworkMessage(other), cell1Id(0), cell2Id(0), polypeptideId1(0), polypeptideId2(0), polypeptide1Dies(0), polypeptide2Dies(0)
{ 
	messageType = MessageType::PolypeptideFight;
}

PolypeptideFight::~PolypeptideFight() 
{ }

unsigned PolypeptideFight::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkcell1Id = htonl(cell1Id);
	memcpy(&data[index], &networkcell1Id, sizeof(networkcell1Id));
	index += sizeof(networkcell1Id);

	unsigned networkcell2Id = htonl(cell2Id);
	memcpy(&data[index], &networkcell2Id, sizeof(networkcell2Id));
	index += sizeof(networkcell2Id);

	unsigned networkpolypeptideId1 = htonl(polypeptideId1);
	memcpy(&data[index], &networkpolypeptideId1, sizeof(networkpolypeptideId1));
	index += sizeof(networkpolypeptideId1);

	unsigned networkpolypeptideId2 = htonl(polypeptideId2);
	memcpy(&data[index], &networkpolypeptideId2, sizeof(networkpolypeptideId2));
	index += sizeof(networkpolypeptideId2);

	memcpy(&data[index], &polypeptide1Dies, sizeof(polypeptide1Dies));
	index += sizeof(polypeptide1Dies);

	memcpy(&data[index], &polypeptide2Dies, sizeof(polypeptide2Dies));
	index += sizeof(polypeptide2Dies);
	
	return index;
}

unsigned PolypeptideFight::calculateSize()
{
	return NetworkMessage::calculateSize()
		+ sizeof(cell1Id)
		+ sizeof(cell2Id)
		+ sizeof(polypeptideId1)
		+ sizeof(polypeptideId2)
		+ sizeof(polypeptide1Dies)
		+ sizeof(polypeptide2Dies);
}