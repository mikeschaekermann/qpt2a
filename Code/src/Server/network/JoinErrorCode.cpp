#include "JoinErrorCode.h"
#include <boost/asio.hpp>

JoinErrorCode::JoinErrorCode(Type value)
{
	this->value = value;
}

JoinErrorCode::JoinErrorCode(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

JoinErrorCode::Type JoinErrorCode::getType()
{
	return Type(value);
}

unsigned int JoinErrorCode::getNetworkType()
{
	return htonl(value);
}