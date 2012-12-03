#include "CreateCellErrorCode.h"
#include <boost/asio.hpp>

CreateCellErrorCode::CreateCellErrorCode(Type value)
{
	this->value = value;
}

CreateCellErrorCode::CreateCellErrorCode(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

CreateCellErrorCode::Type CreateCellErrorCode::getType()
{
	return Type(value);
}

unsigned int CreateCellErrorCode::getNetworkType()
{
	return htonl(value);
}