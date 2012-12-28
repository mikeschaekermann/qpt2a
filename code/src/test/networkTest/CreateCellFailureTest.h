#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"

using namespace std;

void testCreateCellFailure()
{
	cout << "CreateCellFailure Test[Start]" << endl;

	CreateCellFailure *message = new CreateCellFailure();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->requestId = 2234;
	message->errorCode = CreateCellErrorCode::SpotAlreadyTaken;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	CreateCellFailure *received = new CreateCellFailure(data, index);

	cout << "CreateCellFailure Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "CreateCellFailure Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "CreateCellFailure Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "CreateCellFailure Test[Assert 4]" << endl;
	assert(message->requestId == received->requestId);
	
	cout << "CreateCellFailure Test[Assert 5]" << endl;
	assert(message->errorCode.getType() == received->errorCode.getType());

	delete message;
	delete received;
	delete data;
	cout << "CreateCellFailure Test[End]" << endl;
}