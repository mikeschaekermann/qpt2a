#include "JoinRequest.h"

JoinRequest::JoinRequest() : NetworkMessage(), name(0), nameSize(0)
{ }

JoinRequest::JoinRequest(char* data, unsigned &index) : NetworkMessage(data, index), name(0), nameSize(0)
{
	memcpy(&nameSize, (void*) data[index], sizeof(nameSize));
	nameSize = ntohl(nameSize);
	index += sizeof(nameSize);

	name = new char[nameSize];
	memcpy((void*) name, (void*) data[index], sizeof(nameSize));
	index += nameSize;
}

JoinRequest::JoinRequest(const JoinRequest &other) : NetworkMessage(other), name(0), nameSize(other.nameSize)
{
	name = new char[nameSize];
	memcpy((void*) name, (void*) other.name, nameSize);
}

JoinRequest::JoinRequest(const NetworkMessage &other) : NetworkMessage(other), name(0), nameSize(0)
{ }

JoinRequest::~JoinRequest()
{
	if (name)
	{
		delete[] name;
	}
}

unsigned JoinRequest::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkNameSize = htonl(nameSize);
	memcpy((void*) data[index], &networkNameSize, sizeof(networkNameSize));
	index += sizeof(networkNameSize);

	
	memcpy((void*) data[index], (void*) name, sizeof(nameSize));
	index += nameSize;
	
	return index;
}

unsigned JoinRequest::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(nameSize)
		+ nameSize;
}