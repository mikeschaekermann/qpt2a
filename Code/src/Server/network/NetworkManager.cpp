#include "NetworkManager.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <vector>

#include "JoinRequest.h"
#include "CreateCellRequest.h"
#include "../game/Game.h"
#include "../game/Player.h"

using boost::asio::ip::udp;
using namespace std;

NetworkManager::NetworkManager(unsigned short listenPort, Game *game) : serverSocket(io_service, udp::endpoint(udp::v4(), listenPort)), game(game)
{
	
}

NetworkManager::NetworkManager(const NetworkManager &other) : serverSocket(io_service, other.serverSocket.local_endpoint()), game(other.game)
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
		NetworkMessage *message = createNetworkMessage(buffer.c_array());

		message->endpoint = remote_endpoint;

		if (!dynamic_cast<JoinRequest*>(message))
		{
			Player *player = getPlayer(message->endpoint);
			if (player)
			{
				for (unsigned i = player->m_uiClientPacketId + 1; i < message->messageId; ++i)
				{
					player->m_unreceivedMessages.push(i);
				}

				player->m_uiClientPacketId = message->messageId;
			}
			else
			{
				// throw exception
			}
		}

		// React to the Message and delete it.
		handleMessage(message);
		delete message;
	}
}

void NetworkManager::handleMessage(NetworkMessage* message) {
	JoinRequest *joinRequest = dynamic_cast<JoinRequest*>(message);
	if (joinRequest) 
	{
		// do Stuff
	}

	CreateCellRequest *createCellRequest = dynamic_cast<CreateCellRequest*>(message);
	if (createCellRequest) 
	{
		// do Stuff
	}
}



	
NetworkMessage* NetworkManager::createNetworkMessage(char* data)
{
	unsigned messageType;
	memcpy(&messageType, (void*) data[sizeof(unsigned)], sizeof(unsigned));
	MessageType type(messageType);
		
	NetworkMessage *message = 0;


	unsigned index = 0;
	switch (type.getType())
	{
	case MessageType::JoinRequest:
		{
				
			message = new JoinRequest(data, index);
			break;
		}
	case MessageType::CreateCellRequest:
		{
			message = new CreateCellRequest(data, index);
			break;
		}
	default:
		break;
	}

	return message;
}

void NetworkManager::send(NetworkMessage message)
{
	Player *player = getPlayer(message.endpoint);
	message.messageSize = message.calculateSize();
	
	message.messageId = player->m_uiServerPacketId++;
	


	char* buffer = new char[message.messageSize];

	message.writeToArray(buffer);
	boost::system::error_code ignored_error;

	serverSocket.send_to(boost::asio::buffer(buffer, message.messageSize), message.endpoint, 0, ignored_error);

}

Player* NetworkManager::getPlayer(boost::asio::ip::udp::endpoint endpoint)
{
	for (std::vector<Player>::iterator it = game->m_players.begin(); it != game->m_players.end(); ++it)
	{
		if ((*it).getIpAddress() == endpoint.address().to_string())
		{
			return &(*it);
		}
	}

	return 0;
}