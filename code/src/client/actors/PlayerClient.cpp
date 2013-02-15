#include "PlayerClient.h"

PlayerClient::PlayerClient(unsigned int id, string name, bool canManipulate) :
	Player(id, name),
	m_bCanManipulate(canManipulate),
	hue(0.5)
{
	initializeHue();
}

PlayerClient::PlayerClient(string name, bool canManipulate) :
	Player(0, name),
	m_bCanManipulate(canManipulate),
	hue(0.5)
{
	initializeHue();
}

PlayerClient::~PlayerClient(void)
{
}

bool PlayerClient::operator==(const PlayerClient & otherPlayer) const
{
	return (getId() == otherPlayer.getId());
}

void PlayerClient::initializeHue()
{
	/// initializes hue value pseudo-randomly
	/// according to the name which is unique among
	/// all players in a game

	unsigned int srandValue = 0;

	for (unsigned int i = 0; i < name.length(); ++i)
	{
		srandValue = srandValue << 4;
		srandValue = name[i] + (name[i] ^ srandValue);
	}

	srand(srandValue);

	hue = float(rand() % 101) / 100.0f;

	srand((unsigned int) time(NULL));
}