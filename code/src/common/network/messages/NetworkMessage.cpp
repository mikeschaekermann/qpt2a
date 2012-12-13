#include "NetworkMessage.h"

NetworkMessage::NetworkMessage() : messageType(MessageType::Invalid)
{

}

NetworkMessage::NetworkMessage(const NetworkMessage &other) : messageType(MessageType::Invalid), endpoint(other.endpoint)
{ 

}

NetworkMessage::NetworkMessage(char* data, unsigned &index) : messageType(MessageType::Invalid)
{
	memcpy(&messageSize, &data[index], sizeof(messageSize));
	messageSize = ntohl(messageSize);
	index += sizeof(messageSize);

	unsigned networkMessageType = 0;
	memcpy(&networkMessageType, &data[index], sizeof(networkMessageType));
	messageType = MessageType(networkMessageType);
	index += sizeof(networkMessageType);
	
	memcpy(&messageId, &data[index], sizeof(messageId));
	messageId = ntohl(messageId);
	index += sizeof(messageId);
}

NetworkMessage::~NetworkMessage()
{

}

unsigned NetworkMessage::calculateSize()
{
	return sizeof(messageSize)
		+ sizeof(messageId) 
		+ sizeof(messageType);
}

unsigned NetworkMessage::writeToArray(char* data, unsigned start)
{
	unsigned index = start;

	unsigned networkMessageSize = htonl(messageSize);
	memcpy(&data[index], &networkMessageSize, sizeof(networkMessageSize));
	index += sizeof(networkMessageSize);

	unsigned networkMessageType = messageType.getNetworkType();
	memcpy(&data[index], &networkMessageType, sizeof(networkMessageType));
	index += sizeof(networkMessageType);

	int networkMessageId = htonl(messageId);
	memcpy(&data[index], &messageId, sizeof(messageId));
	index += sizeof(messageId);

	return index;
}