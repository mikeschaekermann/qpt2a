#include "JoinFailure.h"

JoinFailure::JoinFailure() : NetworkMessage(), errorCode(JoinErrorCode::Invalid)
{
	messageType = MessageType::JoinFailure;
}

JoinFailure::JoinFailure(char* data, unsigned &index) : NetworkMessage(data, index), errorCode(JoinErrorCode::Invalid)
{
	unsigned networkErrorCode = 0;
	memcpy(&networkErrorCode, (void*) data[index], sizeof(networkErrorCode));
	errorCode = JoinErrorCode(networkErrorCode);
	index += sizeof(networkErrorCode);
}

JoinFailure::JoinFailure(const JoinFailure &other) : NetworkMessage(other), errorCode(JoinErrorCode::Invalid)
{
	messageType = MessageType::JoinFailure;
}

JoinFailure::JoinFailure(const NetworkMessage &other) : NetworkMessage(other), errorCode(JoinErrorCode::Invalid)
{ 
	messageType = MessageType::JoinFailure;
}

JoinFailure::~JoinFailure() 
{ }

unsigned JoinFailure::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkErrorCode = errorCode.getNetworkType();
	memcpy((void*) data[index], &networkErrorCode, sizeof(networkErrorCode));
	index += sizeof(networkErrorCode);
	
	return index;
}

unsigned JoinFailure::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(errorCode.getNetworkType());
}