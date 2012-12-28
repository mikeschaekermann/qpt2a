#include <cassert>
#include <iostream>

#include "../../common/network/messages/ConnectionMessage.h"

using namespace std;

void testConnectionMessage()
{
	cout << "ConnectionMessage Test[Start]" << endl;

	ConnectionMessage *message = new ConnectionMessage();
	message->messageId = 20;
	unsigned missingMessageCount = 3;

	for (unsigned i = 0; i < missingMessageCount; ++i)
	{
		message->missingMessageIds.push_back(20 + 3 * i);
	}
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);

	unsigned index = 0;
	ConnectionMessage *received = new ConnectionMessage(data, index);

	cout << "ConnectionMessage Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "ConnectionMessage Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "ConnectionMessage Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);
	cout << "ConnectionMessage Test[Assert 4]" << endl;
	for (unsigned i = 0; i < message->missingMessageIds.size(); ++i)
	{
		assert(message->missingMessageIds[i] == received->missingMessageIds[i]);
	}
	delete message;
	delete received;
	delete data;
	cout << "ConnectionMessage Test[End]" << endl;
}

