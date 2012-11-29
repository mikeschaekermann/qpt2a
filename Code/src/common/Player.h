#pragma once

#include "Client\Config.h"

class Player
{
public:
	Player(bool canManipulate, unsigned playerId);
	~Player(void);
	
	string getName() const { return m_name; }
	int getPlayerId() const { return m_uiPlayerId; }

private:
	string m_name;
	unsigned m_uiPlayerId;
	bool m_bCanManipulate;
};