#include "JoinRequest.h"

JoinRequest::JoinRequest() : NetworkMessage(), name()
{
	messageType = MessageType::JoinRequest;
}

JoinRequest::JoinRequest(char* data, unsigned &index) : NetworkMessage(data, index), name()
{
	unsigned nameSize;
	memcpy(&nameSize, &data[index], sizeof(nameSize));
	nameSize = ntohl(nameSize);
	index += sizeof(nameSize);

	char *nameArray = new char[nameSize + 1];
	memcpy(nameArray, &data[index], nameSize);
	nameArray[nameSize] = 0;
	name = std::string(nameArray);
	index += nameSize;
}

JoinRequest::JoinRequest(const JoinRequest &other) : NetworkMessage(other), name(other.name)
{
	messageType = MessageType::JoinRequest;
}

JoinRequest::JoinRequest(const NetworkMessage &other) : NetworkMessage(other), name()
{ 
	messageType = MessageType::JoinRequest;
}

JoinRequest::~JoinRequest()
{

}

unsigned JoinRequest::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkNameSize = htonl(name.length());
	memcpy(&data[index], &networkNameSize, sizeof(networkNameSize));
	index += sizeof(networkNameSize);

	const char* nameArray = name.c_str();
	memcpy(&data[index], nameArray, name.length());
	index += name.length();
	
	return index;
}

unsigned JoinRequest::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(unsigned)
		+ name.length();
}