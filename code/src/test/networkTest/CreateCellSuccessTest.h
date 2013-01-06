#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"

using namespace std;

void testCreateCellSuccess()
{
	cout << "CreateCellSuccess Test[Start]" << endl;

	CreateCellSuccess *message = new CreateCellSuccess();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->rotation = ci::Vec3f(123.5f, 2.f, 3.5f);
	message->cellId = 532;
	message->position = ci::Vec3f(234.533f, 0.0f, 23.0f);
	message->requestId = 0;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	CreateCellSuccess *received = new CreateCellSuccess(data, index);

	cout << "CreateCellSuccess Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "CreateCellSuccess Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "CreateCellSuccess Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "CreateCellSuccess Test[Assert 4]" << endl;
	assert(message->rotation == received->rotation);
	cout << "CreateCellSuccess Test[Assert 5]" << endl;
	assert(message->cellId == received->cellId);
	cout << "CreateCellSuccess Test[Assert 6]" << endl;
	assert(message->position == received->position);
	cout << "CreateCellSuccess Test[Assert 7]" << endl;
	assert(message->requestId == received->requestId);

	delete message;
	delete received;
	delete data;
	cout << "CreateCellSuccess Test[End]" << endl;
}