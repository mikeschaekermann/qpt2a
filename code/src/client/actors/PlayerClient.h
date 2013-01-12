#pragma once

#include "../../common/Player.h"

using namespace std;

class PlayerClient:
	public Player
{
public:
	PlayerClient(unsigned int id, string name, bool canManipulate = false);
	PlayerClient(string name, bool canManipulate = false);
	~PlayerClient(void);

	bool operator==(const PlayerClient & otherPlayer) const;

private:
	bool m_bCanManipulate;
};