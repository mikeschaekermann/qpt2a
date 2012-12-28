#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"

using namespace std;

void testCellDie()
{
	cout << "CellDie Test[Start]" << endl;

	CellDie *message = new CellDie();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->cellId = 7;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	CellDie *received = new CellDie(data, index);

	cout << "CellDie Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "CellDie Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "CellDie Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "CellDie Test[Assert 4]" << endl;
	assert(message->cellId == received->cellId);

	delete message;
	delete received;
	delete data;
	cout << "CellDie Test[End]" << endl;
}