#include "StandardCellClient.h"
#include "../rendering/RenderManager.h"
#include "../managers/GameManager.h"
#include <cmath>

void StandardCellClient::startAttackAnimation()
{
	SOUND_PLAYER->playSound(string("cellAttack"), position, Vec3f::zero());
	spikesScale = 1;
}

void StandardCellClient::drawAtTransformation() const
{
	/// draw spikes
	if (spikesCanBeSeen())
	{
		for (auto it = spikesAngleRadiusList.begin(); it != spikesAngleRadiusList.end(); ++it)
		{
			gl::pushMatrices();

			auto spikeAngle = it->first;
			auto spikeRadius = it->second;

			auto currentSpikeRadius = spikesScale * spikeRadius;
			gl::scale(currentSpikeRadius, currentSpikeRadius, currentSpikeRadius);
			gl::rotate(Vec3f(0, 90, 0));
			gl::rotate(Vec3f(spikeAngle, 0, 0));

			RenderManager::getInstance()->renderSlimeShadedModel("standardCellAttack",
													  position,
													  Vec4f(0.1f, 0.1f, 0.1f, 1.0f),
													  Vec4f(0.1f, 0.1f, 0.1f, 1.0f),
													  Vec4f(0.9f, 0.9f, 0.9f, 1.0f),
													  10.f);

			gl::popMatrices();
		}
	}

	/// draw cell body
	gl::pushMatrices();

	auto h = getHue();
	auto s = getSaturation();
	auto b = getBrightnessFromHealthPoints();
	auto o = getOpacityFromHealthPoints();

	ColorA ambientColor(CM_HSV, h, s * 0.4f, b * 0.4f, o * 0.4f);
	ColorA diffuseColor(CM_HSV, h, s * 0.6f, b * 0.6f, o * 0.6f);
	ColorA specularColor(CM_HSV, h, s, b, o);

	gl::scale(radius, radius, radius);
	RenderManager::getInstance()->renderSlimeShadedModel("standardCell",
											  position,
											  Vec4f(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a),
											  Vec4f(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a),
											  Vec4f(specularColor.r, specularColor.g, specularColor.b, specularColor.a),
											  4.f);

	gl::popMatrices();
}

void StandardCellClient::update(float frameTime)
{
	CellClient::update(frameTime);

	if (spikesCanBeSeen())
	{
		spikesScale = spikesScale * pow(CONFIG_FLOAT("data.cell.standardcell.spikes.shrinkAmountPerSecond"), frameTime);
	}
}

bool StandardCellClient::spikesCanBeSeen() const
{
	return (spikesScale * spikesAngleRadiusList.begin()->second >= radius);
}

void StandardCellClient::increaseGlobalTypeCounter() const
{
	POLYCAPACITY->changeNumberOfStandardCells(1);
}

void StandardCellClient::decreaseGlobalTypeCounter() const
{
	POLYCAPACITY->changeNumberOfStandardCells(-1);
}