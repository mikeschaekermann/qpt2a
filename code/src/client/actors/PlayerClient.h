#pragma once

#include "../../common/Player.h"
#include "../actors/CellClient.h"
#include "../rendering/MarchingCubes.h"
#include "cinder/Sphere.h"

using namespace std;

class PlayerClient:
	public Player
{
public:
	PlayerClient(unsigned int id, string name, bool canManipulate = false);
	PlayerClient(string name, bool canManipulate = false);
	~PlayerClient(void);

	bool operator==(const PlayerClient & otherPlayer) const;

	virtual CellClient & getStemCell() { return dynamic_cast<CellClient &>(*stemCell); }

	float getHue() const { return hue; }
	void setHue(float newHue) { hue = min<float>(max<float>(newHue, 0.0), 1.0); }
	
	MarchingCubes & getSkin();
	void drawSkin();
	void addSkinBubble(Sphere bubble);
	void removeSkinBubble(Sphere bubble);

private:
	MarchingCubes skin;
	bool m_bCanManipulate;
	float hue;

	void initializeHue();
};