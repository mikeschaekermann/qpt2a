#include "ConnectionMessage.h"

ConnectionMessage::ConnectionMessage() : NetworkMessage()
{
	messageType = MessageType::ConnectionMessage;
}

ConnectionMessage::ConnectionMessage(char* data, unsigned &index) : NetworkMessage(data, index)
{
	unsigned missingMessageCount;
	memcpy(&missingMessageCount, &data[index], sizeof(missingMessageCount));
	missingMessageCount = ntohl(missingMessageCount);
	index += sizeof(missingMessageCount);

	for (unsigned i = 0; i < missingMessageCount; ++i)
	{
		unsigned currentId;
		memcpy(&currentId, &data[index], sizeof(messageId));
		missingMessageIds.push_back(ntohl(currentId));
		index += sizeof(messageId);
	}
}

ConnectionMessage::ConnectionMessage(const ConnectionMessage &other) : NetworkMessage(other), missingMessageIds(other.missingMessageIds)
{ 
	messageType = MessageType::ConnectionMessage;
}

ConnectionMessage::ConnectionMessage(const NetworkMessage &other) : NetworkMessage(other)
{ 
	messageType = MessageType::ConnectionMessage;
}

ConnectionMessage::~ConnectionMessage() 
{ 
}

unsigned ConnectionMessage::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);

	unsigned networkMissingMessageCount = htonl(missingMessageIds.size());
	memcpy(&data[index], &networkMissingMessageCount, sizeof(networkMissingMessageCount));
	index += sizeof(networkMissingMessageCount);

	unsigned *networkMessageIds = new unsigned[missingMessageIds.size()];
	for (unsigned i = 0; i < missingMessageIds.size(); ++i)
	{
		networkMessageIds[i] = htonl(missingMessageIds[i]);
	}
	memcpy(&data[index], networkMessageIds, missingMessageIds.size() * sizeof(messageId));
	delete[] networkMessageIds;
	index += missingMessageIds.size() * sizeof(messageId);
	
	return index;
}

unsigned ConnectionMessage::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(unsigned)
		+ missingMessageIds.size() * sizeof(messageId);
}