#include "PlayerClient.h"
#include "../rendering/RenderManager.h"
#include "boost/thread.hpp"

PlayerClient::PlayerClient(unsigned int id, string name, bool canManipulate) :
	Player(id, name),
	m_bCanManipulate(canManipulate),
	hue(0.5),
	skin(
		Vec3f(
			2 * CONFIG_FLOAT("data.world.radius"),
			2 * CONFIG_FLOAT("data.world.radius"),
			200
		),
		Vec3f(
			CONFIG_FLOAT("data.skin.grid.cell.size"),
			CONFIG_FLOAT("data.skin.grid.cell.size"),
			CONFIG_FLOAT("data.skin.grid.cell.size")
		),
		CONFIG_FLOAT("data.skin.grid.isoLevel")
	)
{
	initializeHue();
}

PlayerClient::PlayerClient(string name, bool canManipulate) :
	Player(0, name),
	m_bCanManipulate(canManipulate),
	hue(0.5),
	skin(
		Vec3f(
			2 * CONFIG_FLOAT("data.world.radius"),
			2 * CONFIG_FLOAT("data.world.radius"),
			200
		),
		Vec3f(
			CONFIG_FLOAT("data.skin.grid.cell.size"),
			CONFIG_FLOAT("data.skin.grid.cell.size"),
			CONFIG_FLOAT("data.skin.grid.cell.size")
		),
		CONFIG_FLOAT("data.skin.grid.isoLevel")
	)
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

void PlayerClient::drawSkin()
{
	if (skin.meshExists())
	{
		auto ambientColor = Color(CM_HSV, hue, 0.8f, 1.f);
		auto diffuseColor = Color(CM_HSV, hue, 0.7f, 0.4f);
		
		skin.getMeshMutex().lock();
		auto skinMesh = skin.getMesh();
		skin.getMeshMutex().unlock();

		RENDER_MGR->renderSkin(
			skinMesh,
			Vec4f(ambientColor.r, ambientColor.g, ambientColor.b, 0.15f),
			Vec4f(diffuseColor.r, diffuseColor.g, diffuseColor.b, 0.9f),
			Vec4f(1.f, 1.f, 1.f, 0.9f),
			5.f
		);

		///////////// opaque configuration
		/*RENDER_MGR->renderSkin(
			skin.getMesh(),
			Vec4f(ambientColor.r, ambientColor.g, ambientColor.b, 0.9f),
			Vec4f(diffuseColor.r, diffuseColor.g, diffuseColor.b, 0.8f),
			Vec4f(1.f, 1.f, 1.f, 1.f),
			5.f
		);*/
	}
}

void PlayerClient::addSkinBubble(Sphere bubble)
{
	boost::thread(boost::bind(&MarchingCubes::addMetaballSphere, &skin, bubble));
}

void PlayerClient::removeSkinBubble(Sphere bubble)
{
	boost::thread(boost::bind(&MarchingCubes::removeMetaballSphere, &skin, bubble));
}