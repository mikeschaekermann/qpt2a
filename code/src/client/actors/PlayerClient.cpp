#include "PlayerClient.h"

PlayerClient::PlayerClient(unsigned int id, string name, bool canManipulate) :
	Player(id, name),
	m_bCanManipulate(canManipulate)
{
}

PlayerClient::PlayerClient(string name, bool canManipulate) :
	Player(0, name),
	m_bCanManipulate(canManipulate)
{
}

PlayerClient::~PlayerClient(void)
{
}

bool PlayerClient::operator==(const PlayerClient & otherPlayer) const
{
	return (getId() == otherPlayer.getId());
}