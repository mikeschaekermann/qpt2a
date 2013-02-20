#include "MovePolypeptideRequest.h"

MovePolypeptideRequest::MovePolypeptideRequest() : NetworkMessage(), requestId(0), fromCellId(0), toCellId(0), amount(0)
{
	messageType = MessageType::MovePolypeptideRequest;
}

MovePolypeptideRequest::MovePolypeptideRequest(char* data, unsigned &index) : NetworkMessage(data, index), requestId(0), fromCellId(0), toCellId(0), amount(0)
{
	memcpy(&requestId, &data[index], sizeof(requestId));
	requestId = ntohl(requestId);
	index += sizeof(requestId);

	memcpy(&fromCellId, &data[index], sizeof(fromCellId));
	fromCellId = ntohl(fromCellId);
	index += sizeof(fromCellId);

	memcpy(&toCellId, &data[index], sizeof(toCellId));
	toCellId = ntohl(toCellId);
	index += sizeof(toCellId);

	memcpy(&amount, &data[index], sizeof(amount));
	amount = ntohl(amount);
	index += sizeof(amount);
}

MovePolypeptideRequest::MovePolypeptideRequest(const MovePolypeptideRequest &other) : NetworkMessage(other), 
	requestId(other.requestId), fromCellId(other.fromCellId), toCellId(other.toCellId), amount(other.amount)
{
	messageType = MessageType::MovePolypeptideRequest;
}

MovePolypeptideRequest::MovePolypeptideRequest(const NetworkMessage &other) : NetworkMessage(other), requestId(0), fromCellId(0), toCellId(0), amount(0)
{ 
	messageType = MessageType::MovePolypeptideRequest;
}

MovePolypeptideRequest::~MovePolypeptideRequest() 
{ }

unsigned MovePolypeptideRequest::writeToArray(char* data, unsigned start)
{
	unsigned index = NetworkMessage::writeToArray(data);
	
	unsigned networkrequestId = htonl(requestId);
	memcpy(&data[index], &networkrequestId, sizeof(networkrequestId));
	index += sizeof(networkrequestId);

	unsigned networkFromCellId = htonl(fromCellId);
	memcpy(&data[index], &networkFromCellId, sizeof(networkFromCellId));
	index += sizeof(networkFromCellId);

	unsigned networktoCellId = htonl(toCellId);
	memcpy(&data[index], &networktoCellId, sizeof(networktoCellId));
	index += sizeof(networktoCellId);

	unsigned networkamount = htonl(amount);
	memcpy(&data[index], &networkamount, sizeof(networkamount));
	index += sizeof(networkamount);

	return index;
}

unsigned MovePolypeptideRequest::calculateSize()
{
	return NetworkMessage::calculateSize() 
		+ sizeof(requestId)
		+ sizeof(fromCellId)
		+ sizeof(toCellId)
		+ sizeof(amount);
}
