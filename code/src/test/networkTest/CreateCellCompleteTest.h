#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"

using namespace std;

void testCreateCellComplete()
{
	cout << "CreateCellComplete Test[Start]" << endl;

	CreateCellComplete *message = new CreateCellComplete();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->cellId = 325;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	CreateCellComplete *received = new CreateCellComplete(data, index);

	cout << "CreateCellComplete Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "CreateCellComplete Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "CreateCellComplete Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "CreateCellComplete Test[Assert 4]" << endl;
	assert(message->cellId == received->cellId);

	delete message;
	delete received;
	delete data;
	cout << "CreateCellComplete Test[End]" << endl;
}