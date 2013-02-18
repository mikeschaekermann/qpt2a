#include "PlayerClient.h"
#include "../rendering/RenderManager.h"

PlayerClient::PlayerClient(unsigned int id, string name, bool canManipulate) :
	Player(id, name),
	m_bCanManipulate(canManipulate),
	hue(0.5),
	skin(Vec3f(2 * CONFIG_FLOAT1("data.world.radius"), 2 * CONFIG_FLOAT1("data.world.radius"), 200), Vec3f(15.f, 15.f, 15.f), 100.f)
{
	initializeHue();
}

PlayerClient::PlayerClient(string name, bool canManipulate) :
	Player(0, name),
	m_bCanManipulate(canManipulate),
	hue(0.5),
	skin(Vec3f(2 * CONFIG_FLOAT1("data.world.radius"), 2 * CONFIG_FLOAT1("data.world.radius"), 200), Vec3f(15.f, 15.f, 15.f), 100.f)
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
		auto ambientColor = Color(CM_HSV, hue, 0.8f, 1.f);
		auto diffuseColor = Color(CM_HSV, hue, 0.7f, 0.4f);
		
		RENDER_MGR->renderPhongShadedModel(
			skin.getMesh(),
			Vec4f(ambientColor.r, ambientColor.g, ambientColor.b, 0.15f),
			Vec4f(diffuseColor.r, diffuseColor.g, diffuseColor.b, 0.9f),
			Vec4f(1.f, 1.f, 1.f, 0.9f),
			5.f
		);
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