#include "CreatePolypeptideErrorCode.h"
#include <boost/asio.hpp>

CreatePolypeptideErrorCode::CreatePolypeptideErrorCode(Type value)
{
	this->value = value;
}

CreatePolypeptideErrorCode::CreatePolypeptideErrorCode(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

CreatePolypeptideErrorCode::Type CreatePolypeptideErrorCode::getType()
{
	return Type(value);
}

unsigned int CreatePolypeptideErrorCode::getNetworkType()
{
	return htonl(value);
}