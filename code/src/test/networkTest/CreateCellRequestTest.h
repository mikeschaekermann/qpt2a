#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"

using namespace std;

void testCreateCellRequest()
{
	cout << "CreateCellRequest Test[Start]" << endl;

	CreateCellRequest *message = new CreateCellRequest();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->rotation = ci::Vec3f(123.45f, 32.43f, 0.f);
	message->cellId = 67;
	message->playerId = 1;
	message->requestId = 234;
	message->type = CellType::StandardCell;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	CreateCellRequest *received = new CreateCellRequest(data, index);

	cout << "CreateCellRequest Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "CreateCellRequest Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "CreateCellRequest Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "CreateCellRequest Test[Assert 4]" << endl;
	assert(message->type.getType() == received->type.getType());

	cout << "CreateCellRequest Test[Assert 5]" << endl;
	assert(message->cellId == received->cellId);

	cout << "CreateCellRequest Test[Assert 6]" << endl;
	assert(message->requestId == received->requestId);

	cout << "CreateCellRequest Test[Assert 7]" << endl;
	assert(message->rotation == received->rotation);

	cout << "CreateCellRequest Test[Assert 8]" << endl;
	assert(message->playerId == received->playerId);

	delete message;
	delete received;
	delete data;
	cout << "CreateCellRequest Test[End]" << endl;
}