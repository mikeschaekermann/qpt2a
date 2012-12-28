#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/outgame/JoinSuccess.h"

using namespace std;

void testJoinSuccess()
{
	cout << "JoinSuccess Test[Start]" << endl;

	JoinSuccess *message = new JoinSuccess();
	message->messageId = 32;
	message->messageType = MessageType::JoinSuccess;
	message->playerId = 3;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	JoinSuccess *received = new JoinSuccess(data, index);

	cout << "JoinSuccess Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "JoinSuccess Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "JoinSuccess Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);
	cout << "JoinSuccess Test[Assert 4]" << endl;
	assert(message->playerId == received->playerId);
	delete message;
	delete received;
	delete data;
	cout << "JoinSuccess Test[End]" << endl;
}