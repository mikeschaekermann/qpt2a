#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/outgame/JoinRequest.h"

using namespace std;

void testJoinRequest()
{
	cout << "JoinRequest Test[Start]" << endl;

	JoinRequest *message = new JoinRequest();
	message->messageId = 32;
	message->messageType = MessageType::JoinRequest;

	message->name = "Test";
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	JoinRequest *received = new JoinRequest(data, index);

	cout << "JoinRequest Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "JoinRequest Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "JoinRequest Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);
	cout << "JoinRequest Test[Assert 4]" << endl;
	assert(message->name == received->name);
	delete message;
	delete received;
	delete data;
	cout << "JoinRequest Test[End]" << endl;
}