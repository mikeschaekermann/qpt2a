#pragma once
#define _WIN32_WINNT 0x0501
#include "MessageType.h"
#include <boost/asio.hpp>

class NetworkMessage
{
public:
	/* Fields */
	unsigned int messageSize;											/// The size of the whole networkmessage
	MessageType messageType;											/// The type distinguishes which constructor should be used
	unsigned int messageId;												/// The messageId is to determine whether a message got lost. Works for 64 gb of datatransfer. 
	boost::asio::ip::udp::endpoint endpoint;							/// The source address of the message
	
	/* Constructor / Destructor */
	NetworkMessage();													/// Default constructor
	NetworkMessage(char* data, unsigned &index);						/// Construct from buffer starting by the given index
	NetworkMessage(const NetworkMessage &other);						/// Copy constructor which copies the network address
	virtual ~NetworkMessage();											/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};