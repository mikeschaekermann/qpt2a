#include "NetworkManager.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <vector>

#include "JoinRequest.h"

using boost::asio::ip::udp;
using namespace std;

NetworkManager::NetworkManager(unsigned short listenPort) : serverSocket(io_service, udp::endpoint(udp::v4(), listenPort))
{
	
}

NetworkManager::~NetworkManager()
{

}
		
void NetworkManager::operator()()
{
	while (1)
	{
		boost::array<unsigned, 1> size_buffer;
		udp::endpoint remote_endpoint;
		boost::system::error_code error;
		
		// Receive the message size
		serverSocket.receive_from(boost::asio::buffer(size_buffer), remote_endpoint, 0, error);
		
		if (error && error != boost::asio::error::message_size)
		{
			throw boost::system::system_error(error);
		}

		size_buffer[0] = ntohl(size_buffer[0]);
		if (size_buffer[0] < 0)
		{
			size_buffer[0] = 0;
		}
		char* data = new char[size_buffer[0]];

		// Receive the rest of the message
		serverSocket.receive_from(boost::asio::buffer(data, size_buffer[0]), remote_endpoint, 0, error);
		
		if (error && error != boost::asio::error::message_size)
		{
			throw boost::system::system_error(error);
		}

		// Distinguish the messages by their type
		unsigned messageType;
		memcpy(&messageType, data, sizeof(unsigned));
		MessageType type(messageType);

		switch (type.getType())
		{
		case MessageType::JoinRequest:
			{
				unsigned index = 0;
				JoinRequest request(data, index);
				break;
			}
		case MessageType::CreateCellRequest:
			{

				break;
			}
		default:
			break;
		}
	}
}


void NetworkManager::send(NetworkMessage message)
{
	message.messageSize = message.calculateSize();

	char* buffer = new char[message.messageSize];

	message.writeToArray(buffer);

	udp::endpoint clientEndpoint(message.ipAddress, message.port);
	boost::system::error_code ignored_error;

	serverSocket.send_to(boost::asio::buffer(buffer, message.messageSize), clientEndpoint, 0, ignored_error);
}