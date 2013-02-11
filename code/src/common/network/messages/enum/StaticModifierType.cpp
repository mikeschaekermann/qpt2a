#include "StaticModifierType.h"
#include <boost/asio.hpp>

StaticModifierType::StaticModifierType(Type value)
{
	this->value = value;
}

StaticModifierType::StaticModifierType(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

StaticModifierType::Type StaticModifierType::getType()
{
	return Type(value);
}

unsigned int StaticModifierType::getNetworkType()
{
	return htonl(value);
}