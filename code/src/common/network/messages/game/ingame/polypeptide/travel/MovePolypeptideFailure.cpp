#include "MovePolypeptideFailure.h"

MovePolypeptideFailure::MovePolypeptideFailure() : NetworkMessage(), requestId(0), errorCode(MovePolypeptideErrorCode::Invalid)
{
	messageType = MessageType::MovePolypeptideFailure;
}

MovePolypeptideFailure::MovePolypeptideFailure(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), errorCode(MovePolypeptideErrorCode::Invalid)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	unsigned networkErrorCode = 0;
	memcpy(&networkErrorCode, &data[index], sizeof(networkErrorCode));
	errorCode = MovePolypeptideErrorCode(networkErrorCode);
	index += sizeof(networkErrorCode);
}

MovePolypeptideFailure::MovePolypeptideFailure(const MovePolypeptideFailure &other) : NetworkMessage(other), requestId(other.requestId), errorCode(MovePolypeptideErrorCode::Invalid)
{
	messageType = MessageType::MovePolypeptideFailure;
}

MovePolypeptideFailure::MovePolypeptideFailure(const NetworkMessage &other) : NetworkMessage(other), requestId(0), errorCode(MovePolypeptideErrorCode::Invalid)
{ 
	messageType = MessageType::MovePolypeptideFailure;
}

MovePolypeptideFailure::~MovePolypeptideFailure() 
{ }

unsigned MovePolypeptideFailure::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned networkErrorCode = 0;
	memcpy(&networkErrorCode, &data[index], sizeof(networkErrorCode));
	errorCode = MovePolypeptideErrorCode(networkErrorCode);
	index += sizeof(networkErrorCode);
	
	return index;
}

unsigned MovePolypeptideFailure::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(errorCode);
}
