#include "MovePolypeptideErrorCode.h"
#include <boost/asio.hpp>

MovePolypeptideErrorCode::MovePolypeptideErrorCode(Type value)
{
	this->value = value;
}

MovePolypeptideErrorCode::MovePolypeptideErrorCode(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

MovePolypeptideErrorCode::Type MovePolypeptideErrorCode::getType()
{
	return Type(value);
}

unsigned int MovePolypeptideErrorCode::getNetworkType()
{
	return htonl(value);
}