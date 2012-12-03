#include "Player.h"


Player::Player(bool canManipulate, unsigned playerId) :
	m_bCanManipulate(canManipulate),
	m_uiPlayerId(playerId)
{
}


Player::~Player(void)
{
}
