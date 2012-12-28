#include <cassert>
#include <iostream>

#include "../../common/network/messages/NetworkMessage.h"

using namespace std;

void testNetworkMessage()
{
	cout << "NetworkMessage Test[Start]" << endl;

	NetworkMessage *message = new NetworkMessage();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	NetworkMessage *received = new NetworkMessage(data, index);

	cout << "NetworkMessage Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "NetworkMessage Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "NetworkMessage Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);
	delete message;
	delete received;
	delete data;
	cout << "NetworkMessage Test[End]" << endl;
}