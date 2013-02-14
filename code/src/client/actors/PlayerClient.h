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
	float getHue() const { return hue; }
	void setHue(float newHue) { hue = min<float>(max<float>(newHue, 0.0), 1.0); }

private:
	void initializeHue();

	bool m_bCanManipulate;
	float hue;
};