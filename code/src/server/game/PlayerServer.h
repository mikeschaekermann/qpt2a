#pragma once

#include <string>
#include <vector>
#include <map>

#include "../../common/network/ConnectionEndpoint.h"
#include "../../common/Player.h"
#include "IIdentifiable.h"

using namespace std;

class PlayerServer :
	public Player,
	public ConnectionEndpoint,
	public IIdentifiable
{	
public:
	PlayerServer(unsigned int id, string name, boost::asio::ip::udp::endpoint endpoint, Vec3f position) :
		Player(id, name),  
		ConnectionEndpoint(endpoint),
		stemCell(new StemCell(GameObjectServer::getNewId()))
	{
		stemCell->setPosition(position);
	}
	
	const boost::asio::ip::udp::endpoint& getEndpoint() const { return m_endpoint; }
	StemCell const & getStemCell() const { return *stemCell; }

private:
	StemCell* stemCell;
};