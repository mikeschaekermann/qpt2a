#include "PlayerClient.h"

PlayerClient::PlayerClient(unsigned int id, string name, bool canManipulate) :
	Player(id, name),
	m_bCanManipulate(canManipulate),
	hue(0.5),
	skin(Vec3f(2 * CONFIG_FLOAT1("data.world.radius"), 2 * CONFIG_FLOAT1("data.world.radius"), 200), Vec3f(20.f, 20.f, 20.f), 10.f)
{
	initializeHue();
}

PlayerClient::PlayerClient(string name, bool canManipulate) :
	Player(0, name),
	m_bCanManipulate(canManipulate),
	hue(0.5),
	skin(Vec3f(2 * CONFIG_FLOAT1("data.world.radius"), 2 * CONFIG_FLOAT1("data.world.radius"), 200), Vec3f(20.f, 20.f, 20.f), 10.f)
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
	/// according to the name which is unique
	/// among all players in a game

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

MarchingCubes & PlayerClient::getSkin()
{
	return skin;
}

void PlayerClient::drawSkin() const
{
	if (skin.meshExists())
	{
		gl::color(ColorA(CM_HSV, hue, 1.f, 1.f, 0.5f));
		gl::draw(skin.getMesh());
	}
}

void PlayerClient::addSkinBubble(Sphere bubble)
{
	skin.addMetaball(bubble);
}

void PlayerClient::removeSkinBubble(Sphere bubble)
{
	skin.removeMetaball(bubble);
}