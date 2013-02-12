#pragma once

#include <string>
#include <vector>
#include <map>

#include "../../common/network/ConnectionEndpoint.h"
#include "../../common/Player.h"
#include "IIdentifiable.h"
#include "CellServer.h"

using namespace std;

class PlayerServer :
	public Player,
	public ConnectionEndpoint,
	public IIdentifiable
{
public:
	PlayerServer(string name, boost::asio::ip::udp::endpoint endpoint, Vec3f position) :
		Player(IIdentifiable::getNewId(), name),  
		ConnectionEndpoint(endpoint),
		dead(false)
	{ 
		stemCell = new CellServer(CellServer::STEMCELL, position, 0.f, this);
	}
	
	const boost::asio::ip::udp::endpoint & getEndpoint() const { return m_endpoint; }
	CellServer & getStemCell() { return *stemCell; }
	bool isDead() const { return dead; }
	void kill() { dead = true; }
private:
	CellServer * stemCell;
	bool dead;
};