#include "CellType.h"
#include <boost/asio.hpp>

CellType::CellType(Type value)
{
	this->value = value;
}

CellType::CellType(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

CellType::Type CellType::getType()
{
	return Type(value);
}

unsigned int CellType::getNetworkType()
{
	return htonl(value);
}