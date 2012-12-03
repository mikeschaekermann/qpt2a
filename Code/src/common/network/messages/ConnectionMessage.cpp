#include "ConnectionMessage.h"

ConnectionMessage::ConnectionMessage() : NetworkMessage(), missingMessageCount(0), missingMessageIds(0)
{
	messageType = MessageType::ConnectionMessage;
}

ConnectionMessage::ConnectionMessage(char* data, unsigned &index) : NetworkMessage(data, index), missingMessageCount(0), missingMessageIds(0)
{
	memcpy(&missingMessageCount, (void*) data[index], sizeof(missingMessageCount));
	missingMessageCount = ntohl(missingMessageCount);
	index += sizeof(missingMessageCount);

	missingMessageIds = new unsigned[missingMessageCount];

	memcpy(&missingMessageIds, (void*) data[index], missingMessageCount * sizeof(messageId));

	for (unsigned i = 0; i < missingMessageCount; ++i)
	{
		missingMessageIds[i] = ntohl(missingMessageIds[i]);
	}

	index += sizeof(messageId) * missingMessageCount;
}

ConnectionMessage::ConnectionMessage(const ConnectionMessage &other) : NetworkMessage(other), missingMessageCount(other.missingMessageCount)
{ 
	messageType = MessageType::ConnectionMessage;

	missingMessageIds = new unsigned[missingMessageCount];

	memcpy(&missingMessageIds, (void*) other.missingMessageIds, missingMessageCount * sizeof(messageId));

}

ConnectionMessage::ConnectionMessage(const NetworkMessage &other) : NetworkMessage(other), missingMessageCount(0), missingMessageIds(0)
{ 
	messageType = MessageType::ConnectionMessage;
}

ConnectionMessage::~ConnectionMessage() 
{ 
	if (missingMessageIds)
	{
		delete[] missingMessageIds;
	}
}

unsigned ConnectionMessage::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkMissingMessageCount = htonl(missingMessageCount);
	memcpy((void*) data[index], &networkMissingMessageCount, sizeof(networkMissingMessageCount));
	index += sizeof(networkMissingMessageCount);

	unsigned *networkMessageIds = new unsigned[missingMessageCount];
	for (unsigned i = 0; i < missingMessageCount; ++i)
	{
		networkMessageIds[i] = htonl(missingMessageIds[i]);
	}
	memcpy((void*) data[index], &networkMessageIds, missingMessageCount * sizeof(messageId));
	delete[] networkMessageIds;
	index += missingMessageCount * sizeof(messageId);
	
	return index;
}

unsigned ConnectionMessage::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(missingMessageCount)
		+ sizeof(missingMessageCount) * sizeof(messageId);
}