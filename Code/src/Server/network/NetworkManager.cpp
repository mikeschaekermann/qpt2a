#include "NetworkManager.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "JoinRequest.h"
#include "CreateCellRequest.h"
#include "ConnectionMessage.h"

#include "../game/Game.h"
#include "../game/Player.h"

#include <iostream>

using boost::asio::ip::udp;
using namespace std;

NetworkManager::NetworkManager(unsigned short listenPort, Game *game) :  game(game), 
	run(true)
{
	io_service = new boost::asio::io_service();
	serverSocket = new boost::asio::ip::udp::socket(*io_service, udp::endpoint(udp::v4(), listenPort));
}

NetworkManager::NetworkManager(const NetworkManager &other) : game(other.game), run(true)
{
	io_service = new boost::asio::io_service();
	serverSocket = new boost::asio::ip::udp::socket(*io_service, udp::endpoint(udp::v4(), other.serverSocket->local_endpoint().port()));
}

NetworkManager::~NetworkManager()
{
	run = false;
	serverSocket->shutdown(boost::asio::socket_base::shutdown_both);
	delete serverSocket;
	delete io_service;
}
		
void NetworkManager::operator()()
{
	maintenanceThread = boost::thread(boost::bind(&NetworkManager::connectionMaintenance, this));
	
	boost::array<char, 5000> buffer;
	while (run)
	{
		udp::endpoint remote_endpoint;
		boost::system::error_code error;
		
		// Receive the message
		serverSocket->receive_from(boost::asio::buffer(buffer), remote_endpoint, 0, error);
		maintenanceMutex.lock();
		if (error && error != boost::asio::error::message_size)
		{
			cout << error.message() << endl;
			throw boost::system::system_error(error);
		}
		// Distinguish the messages by their type
		NetworkMessage *message = createNetworkMessage(buffer.c_array());
		message->endpoint = remote_endpoint;

		if (!dynamic_cast<JoinRequest*>(message) && !dynamic_cast<ConnectionMessage*>(message))
		{
			Player *player = getPlayer(message->endpoint);
			if (player)
			{
				for (unsigned i = player->m_uiClientPacketId + 1; i < message->messageId; ++i)
				{
					/// Check whether the messageId is already missing and add it then
					if(std::find(player->m_unreceivedMessages.begin(), player->m_unreceivedMessages.end(), i) == player->m_unreceivedMessages.end()) 
					{
						player->m_unreceivedMessages.push_back(i);
					}
				}
				player->m_unreceivedMessages.remove(message->messageId);
				player->m_uiClientPacketId = message->messageId;
			}
			else
			{
				// throw exception
				assert(false);
			}
		}

		// React to the Message and delete it.
		handleMessage(message);
		delete message;
		maintenanceMutex.unlock();
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

	ConnectionMessage *connectionMessage = dynamic_cast<ConnectionMessage*>(message);
	if (connectionMessage) 
	{
		Player *player = getPlayer(message->endpoint);

		/// Resend all missing messages
		for (unsigned i = 0; i < connectionMessage->missingMessageCount; ++i)
		{
			if (player->m_unconfirmedMessages.find(connectionMessage->missingMessageIds[i]) != player->m_unconfirmedMessages.end())
			{
				baseSend(player->m_unconfirmedMessages[connectionMessage->missingMessageIds[i]]);
			}
			else
			{
				// throw exception
				assert(false);
			}
		}

		/// Remove obsolete messages
		for(std::map<unsigned, NetworkMessage>::iterator it = player->m_unconfirmedMessages.begin(); it != player->m_unconfirmedMessages.end(); ++it) 
		{
			if (it->first <= connectionMessage->messageId)
			{
				bool obsolete = true;
				for (unsigned i = 0; i < connectionMessage->missingMessageCount; ++i)
				{
					if (connectionMessage->missingMessageIds[i] == it->first)
					{
						obsolete = false;
						break;
					}
				}

				if (obsolete)
				{
					player->m_unconfirmedMessages.erase(it->first);
				}
			}
		}
	}
}

void NetworkManager::connectionMaintenance()
{
	while (run)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		maintenanceMutex.lock();
		for (std::vector<Player>::iterator it = game->m_players.begin(); it != game->m_players.end(); ++it)
		{
			ConnectionMessage message;
			// Fill with data
			message.endpoint = (*it).m_endpoint;
			message.messageId = (*it).m_uiClientPacketId;
			message.missingMessageCount = (*it).m_unreceivedMessages.size();

			message.missingMessageIds = new unsigned[message.missingMessageCount];
			unsigned i = 0;
			for (std::list<unsigned>::iterator idIt = (*it).m_unreceivedMessages.begin(); idIt != (*it).m_unreceivedMessages.end(); ++idIt) 
			{
				message.missingMessageIds[i++] = *idIt;
			}

			// Send the message
			baseSend(message);
		}
		maintenanceMutex.unlock();
	}
}

void NetworkManager::stop()
{
	run = false;
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

void NetworkManager::baseSend(NetworkMessage &message)
{
	message.messageSize = message.calculateSize();

	char* buffer = new char[message.messageSize];

	message.writeToArray(buffer);
	boost::system::error_code ignored_error;

	serverSocket->send_to(boost::asio::buffer(buffer, message.messageSize), message.endpoint, 0, ignored_error);

	delete[] buffer;
}

void NetworkManager::send(NetworkMessage message)
{
	maintenanceMutex.lock();
	Player *player = getPlayer(message.endpoint);

	if (player)
	{
		message.messageId = player->m_uiServerPacketId++;
		player->m_unconfirmedMessages[message.messageId] =  message;
	}

	baseSend(message);
	
	maintenanceMutex.unlock();
}

Player* NetworkManager::getPlayer(boost::asio::ip::udp::endpoint endpoint)
{
	for (std::vector<Player>::iterator it = game->m_players.begin(); it != game->m_players.end(); ++it)
	{
		if ((*it).getEndpoint() == endpoint)
		{
			return &(*it);
		}
	}

	return 0;
}