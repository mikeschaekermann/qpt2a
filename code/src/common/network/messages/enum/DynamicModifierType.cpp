#include "DynamicModifierType.h"
#include <boost/asio.hpp>

DynamicModifierType::DynamicModifierType(Type value)
{
	this->value = value;
}

DynamicModifierType::DynamicModifierType(unsigned int networkValue)
{
	value = Type(ntohl(networkValue));
	if (value >= Last_Value)
	{
		value = Invalid;
	}
}

DynamicModifierType::Type DynamicModifierType::getType()
{
	return Type(value);
}

unsigned int DynamicModifierType::getNetworkType()
{
	return htonl(value);
}