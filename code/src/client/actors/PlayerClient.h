#pragma once

#include "../../common/Player.h"

using namespace std;

class PlayerClient:
	public Player
{
public:
	PlayerClient(unsigned int id, string name, Vec2f position, bool canManipulate);
	~PlayerClient(void);
	
private:
	bool m_bCanManipulate;
};