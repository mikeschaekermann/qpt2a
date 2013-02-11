#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif
#include <boost/asio.hpp>

#include "NetworkDynamicModifier.h"
NetworkDynamicModifier::NetworkDynamicModifier() : type(DynamicModifierType::Invalid)
{
}

NetworkDynamicModifier::NetworkDynamicModifier(char* data, unsigned &index) : type(DynamicModifierType::Invalid)
{
	memcpy(&modifierId, &data[index], sizeof(modifierId));
	modifierId = ntohl(modifierId);
	index += sizeof(modifierId);

	memcpy(&position.x, &data[index], sizeof(position.x));
	index += sizeof(position.x);

	memcpy(&position.y, &data[index], sizeof(position.y));
	index += sizeof(position.y);

	memcpy(&position.z, &data[index], sizeof(position.z));
	index += sizeof(position.z);

	memcpy(&rotation.x, &data[index], sizeof(rotation.x));
	index += sizeof(rotation.x);

	memcpy(&rotation.y, &data[index], sizeof(rotation.y));
	index += sizeof(rotation.y);

	memcpy(&rotation.z, &data[index], sizeof(rotation.z));
	index += sizeof(rotation.z);

	memcpy(&scale.x, &data[index], sizeof(scale.x));
	index += sizeof(scale.x);

	memcpy(&scale.y, &data[index], sizeof(scale.y));
	index += sizeof(scale.y);

	memcpy(&scale.z, &data[index], sizeof(scale.z));
	index += sizeof(scale.z);

	memcpy(&radius, &data[index], sizeof(radius));
	index += sizeof(radius);

	unsigned networkType = 0;
	memcpy(&networkType, &data[index], sizeof(networkType));
	type = DynamicModifierType(networkType);
	index += sizeof(networkType);
}

NetworkDynamicModifier::~NetworkDynamicModifier() 
{ 
}

unsigned NetworkDynamicModifier::writeToArray(char* data, unsigned start)
{
	unsigned index = start;
	
	unsigned networkmodifierId = htonl(modifierId);
	memcpy(&data[index], &networkmodifierId, sizeof(networkmodifierId));
	index += sizeof(networkmodifierId);

	memcpy(&data[index], &position.x, sizeof(position.x));
	index += sizeof(position.x);
	
	memcpy(&data[index], &position.y, sizeof(position.y));
	index += sizeof(position.y);

	memcpy(&data[index], &position.z, sizeof(position.z));
	index += sizeof(position.z);

	memcpy(&data[index], &rotation.x, sizeof(rotation.x));
	index += sizeof(rotation.x);
	
	memcpy(&data[index], &rotation.y, sizeof(rotation.y));
	index += sizeof(rotation.y);

	memcpy(&data[index], &rotation.z, sizeof(rotation.z));
	index += sizeof(rotation.z);

	memcpy(&data[index], &scale.x, sizeof(scale.x));
	index += sizeof(scale.x);
	
	memcpy(&data[index], &scale.y, sizeof(scale.y));
	index += sizeof(scale.y);

	memcpy(&data[index], &scale.z, sizeof(scale.z));
	index += sizeof(scale.z);

	memcpy(&data[index], &radius, sizeof(radius));
	index += sizeof(radius);

	unsigned networkType = type.getNetworkType();
	memcpy(&data[index], &networkType, sizeof(networkType));
	index += sizeof(networkType);

	return index;
}

unsigned NetworkDynamicModifier::calculateSize()
{
	unsigned size = sizeof(modifierId) +
		   sizeof(float) * 3 +
		   sizeof(float) * 3 +
		   sizeof(float) * 3 +
		   sizeof(radius) +
		   sizeof(type.getNetworkType());
	
	return size;
}


