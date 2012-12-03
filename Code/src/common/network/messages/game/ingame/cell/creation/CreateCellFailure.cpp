#include "CreateCellFailure.h"

CreateCellFailure::CreateCellFailure() : NetworkMessage(), requestId(0), errorCode(CreateCellErrorCode::Invalid)
{
	messageType = MessageType::CreateCellFailure;
}

CreateCellFailure::CreateCellFailure(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), errorCode(CreateCellErrorCode::Invalid)
{
	memcpy(&requestId, (void*) data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	unsigned networkErrorCode = 0;
	memcpy(&networkErrorCode, (void*) data[index], sizeof(networkErrorCode));
	errorCode = CreateCellErrorCode(networkErrorCode);
	index += sizeof(networkErrorCode);
}

CreateCellFailure::CreateCellFailure(const CreateCellFailure &other) : NetworkMessage(other), requestId(other.requestId), errorCode(CreateCellErrorCode::Invalid)
{
	messageType = MessageType::CreateCellFailure;
}

CreateCellFailure::CreateCellFailure(const NetworkMessage &other) : NetworkMessage(other), requestId(0), errorCode(CreateCellErrorCode::Invalid)
{ 
	messageType = MessageType::CreateCellFailure;
}

CreateCellFailure::~CreateCellFailure() 
{ }

unsigned CreateCellFailure::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy((void*) data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned networkErrorCode = 0;
	memcpy(&networkErrorCode, (void*) data[index], sizeof(networkErrorCode));
	errorCode = CreateCellErrorCode(networkErrorCode);
	index += sizeof(networkErrorCode);
	
	return index;
}

unsigned CreateCellFailure::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(errorCode);
}
