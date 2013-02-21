#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/polypeptide/combat/PolypeptideFight.h"

using namespace std;

void testPolypeptideFight()
{
	cout << "PolypeptideFight Test[Start]" << endl;

	PolypeptideFight *message = new PolypeptideFight();
	message->messageId = 32;
	message->messageType = MessageType::PolypeptideFight;
	message->cell1Id = 8;
	message->cell2Id = 23;
	message->polypeptide1Dies = true;
	message->polypeptide2Dies = false;
	message->polypeptideId1 = 234;
	message->polypeptideId2 = 24;


	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	PolypeptideFight *received = new PolypeptideFight(data, index);

	cout << "PolypeptideFight Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "PolypeptideFight Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "PolypeptideFight Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "PolypeptideFight Test[Assert 4]" << endl;
	assert(message->cell1Id == received->cell1Id);

	
	cout << "PolypeptideFight Test[Assert 5]" << endl;
	assert(message->cell2Id == received->cell2Id);
	
	cout << "PolypeptideFight Test[Assert 6]" << endl;
	assert(message->polypeptide1Dies == received->polypeptide1Dies);
	
	cout << "PolypeptideFight Test[Assert 7]" << endl;
	assert(message->polypeptide2Dies == received->polypeptide2Dies);

	cout << "PolypeptideFight Test[Assert 8]" << endl;
	assert(message->polypeptideId1 == received->polypeptideId1);
	
	cout << "PolypeptideFight Test[Assert 9]" << endl;
	assert(message->polypeptideId2 == received->polypeptideId2);



	delete message;
	delete received;
	delete data;
	cout << "PolypeptideFight Test[End]" << endl;
}