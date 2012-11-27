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

NetworkManager::NetworkManager(const NetworkManager &other)
{

}

NetworkManager::~NetworkManager()
{

}
		
void NetworkManager::operator()()
{
	boost::array<char, 5000> buffer;
	while (1)
	{
		udp::endpoint remote_endpoint;
		boost::system::error_code error;
		
		// Receive the message
		serverSocket.receive_from(boost::asio::buffer(buffer), remote_endpoint, 0, error);
		
		if (error && error != boost::asio::error::message_size)
		{
			throw boost::system::system_error(error);
		}

		// Distinguish the messages by their type
		unsigned messageType;
		memcpy(&messageType, (void*) buffer.c_array()[sizeof(unsigned)], sizeof(unsigned));
		MessageType type(messageType);

		switch (type.getType())
		{
		case MessageType::JoinRequest:
			{
				unsigned index = 0;
				JoinRequest request(buffer.c_array(), index);
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