#include "NetworkMessage.h"

NetworkMessage::NetworkMessage() : messageType(MessageType::Invalid)
{

}

NetworkMessage::NetworkMessage(const NetworkMessage &other) : messageType(MessageType::Invalid), ipAddress(other.ipAddress), port(other.port)
{ 

}

NetworkMessage::NetworkMessage(char* data, unsigned &index) : messageType(MessageType::Invalid)
{
	memcpy(&messageSize, (void*) data[index], sizeof(messageSize));
	messageSize = ntohl(messageSize);
	index += sizeof(messageSize);

	unsigned networkMessageType = 0;
	memcpy(&networkMessageType, (void*) data[index], sizeof(networkMessageType));
	messageType = MessageType(networkMessageType);
	index += sizeof(networkMessageType);
	
	memcpy(&messageId, (void*) data[index], sizeof(messageId));
	messageId = ntohl(messageId);
	index += sizeof(messageId);
}

NetworkMessage::~NetworkMessage()
{

}

unsigned NetworkMessage::calculateSize()
{
	return sizeof(messageId) 
				+ sizeof(messageType);
}

unsigned NetworkMessage::writeToArray(char* data, unsigned start)
{
	unsigned index = start;

	unsigned networkMessageSize = htonl(messageSize);
	memcpy((void*) data[index], &networkMessageSize, sizeof(networkMessageSize));
	index += sizeof(networkMessageSize);

	unsigned networkMessageType = messageType.getNetworkType();
	memcpy((void*) data[index], &networkMessageType, sizeof(networkMessageType));
	index += sizeof(networkMessageType);

	int networkMessageId = htonl(messageId);
	memcpy((void*) data[index], &messageId, sizeof(messageId));
	index += sizeof(messageId);

	return index;
}