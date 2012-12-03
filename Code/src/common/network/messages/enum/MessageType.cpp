#include "MessageType.h"
#include <boost/asio.hpp>

MessageType::MessageType(Type value)
{
	this->value = value;
}

MessageType::MessageType(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

MessageType::Type MessageType::getType()
{
	return Type(value);
}

unsigned int MessageType::getNetworkType()
{
	return htonl(value);
}