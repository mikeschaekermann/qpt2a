#include "PlayerClient.h"

PlayerClient::PlayerClient(unsigned int id, string name, Vec2f position, bool canManipulate) :
	Player(id, name),
	m_bCanManipulate(canManipulate)
{
}

PlayerClient::~PlayerClient(void)
{
}