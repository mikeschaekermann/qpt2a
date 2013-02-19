#include "CreatePolypeptideFailure.h"

CreatePolypeptideFailure::CreatePolypeptideFailure() : NetworkMessage(), requestId(0), errorCode(CreatePolypeptideErrorCode::Invalid)
{
	messageType = MessageType::CreatePolypeptideFailure;
}

CreatePolypeptideFailure::CreatePolypeptideFailure(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), errorCode(CreatePolypeptideErrorCode::Invalid)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	unsigned networkErrorCode = 0;
	memcpy(&networkErrorCode, &data[index], sizeof(networkErrorCode));
	errorCode = CreatePolypeptideErrorCode(networkErrorCode);
	index += sizeof(networkErrorCode);
}

CreatePolypeptideFailure::CreatePolypeptideFailure(const CreatePolypeptideFailure &other) : NetworkMessage(other), requestId(other.requestId), errorCode(CreatePolypeptideErrorCode::Invalid)
{
	messageType = MessageType::CreatePolypeptideFailure;
}

CreatePolypeptideFailure::CreatePolypeptideFailure(const NetworkMessage &other) : NetworkMessage(other), requestId(0), errorCode(CreatePolypeptideErrorCode::Invalid)
{ 
	messageType = MessageType::CreatePolypeptideFailure;
}

CreatePolypeptideFailure::~CreatePolypeptideFailure() 
{ }

unsigned CreatePolypeptideFailure::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned networkErrorCode = 0;
	memcpy(&networkErrorCode, &data[index], sizeof(networkErrorCode));
	errorCode = CreatePolypeptideErrorCode(networkErrorCode);
	index += sizeof(networkErrorCode);
	
	return index;
}

unsigned CreatePolypeptideFailure::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(errorCode);
}
