#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"

using namespace std;

void testCellNew()
{
	cout << "CellNew Test[Start]" << endl;

	CellNew *message = new CellNew();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->rotation = ci::Vec3f(0.f, 57.912f, -0.34f);
	message->cellId = 398;
	message->playerId = 1;
	message->type = CellType::StemCell;
	message->position = ci::Vec3f(12.34f, 56.78f, 910.11f);
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	CellNew *received = new CellNew(data, index);

	cout << "CellNew Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "CellNew Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "CellNew Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "CellNew Test[Assert 4]" << endl;
	assert(message->rotation == received->rotation);
	cout << "CellNew Test[Assert 5]" << endl;
	assert(message->cellId == received->cellId);
	cout << "CellNew Test[Assert 6]" << endl;
	assert(message->playerId == received->playerId);
	cout << "CellNew Test[Assert 7]" << endl;
	assert(message->position == received->position);
	cout << "CellNew Test[Assert 8]" << endl;
	assert(message->type.getType() == received->type.getType());

	delete message;
	delete received;
	delete data;
	cout << "CellNew Test[End]" << endl;
}