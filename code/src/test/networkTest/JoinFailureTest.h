#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/outgame/JoinFailure.h"

using namespace std;

void testJoinFailure()
{
	cout << "JoinFailure Test[Start]" << endl;

	JoinFailure *message = new JoinFailure();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->errorCode = JoinErrorCode::GameIsFull;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	JoinFailure *received = new JoinFailure(data, index);

	cout << "JoinFailure Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "JoinFailure Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "JoinFailure Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "JoinFailure Test[Assert 4]" << endl;
	assert(message->errorCode.getType() == received->errorCode.getType());

	delete message;
	delete received;
	delete data;
	cout << "JoinFailure Test[End]" << endl;
}