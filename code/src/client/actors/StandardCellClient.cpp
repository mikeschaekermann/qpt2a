#include "StandardCellClient.h"
#include "../rendering/RenderManager.h"
#include <cmath>

void StandardCellClient::startAttackAnimation()
{
	spikesScale = 1;
}

void StandardCellClient::drawAtTransformation() const
{
	gl::pushMatrices();

	auto h = getHue();
	auto s = getSaturation();
	auto b = getBrightness();
	auto o = getOpacity();

	ColorA ambientColor(CM_HSV, h, s * 0.4, b * 0.4, o * 0.4);
	ColorA diffuseColor(CM_HSV, h, s * 0.6, b * 0.6, o * 0.6);
	ColorA specularColor(CM_HSV, h, s, b, o);

	gl::scale(radius, radius, radius);
	RenderManager::getInstance()->renderModel("standardCell", "test",
											  Vec4f(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a),
											  Vec4f(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a),
											  Vec4f(specularColor.r, specularColor.g, specularColor.b, specularColor.a),
											  4.);

	gl::popMatrices();

	gl::translate(0, 0, - radius * 0.3);

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

			RenderManager::getInstance()->renderModel("standardCellAttack", "test",
													  Vec4f(0.1, 0.1, 0.1, 1.0),
													  Vec4f(0.1, 0.1, 0.1, 1.0),
													  Vec4f(0.9, 0.9, 0.9, 1.0),
													  10.);

			gl::popMatrices();
		}
	}
}

void StandardCellClient::update(float frameTime)
{
	if (spikesCanBeSeen())
	{
		spikesScale = spikesScale * pow(CONFIG_FLOAT2("data.cell.standardcell.spikes.shrinkAmountPerSecond", 0.5), frameTime);
	}
}

bool StandardCellClient::spikesCanBeSeen() const
{
	return (spikesScale * spikesAngleRadiusList.begin()->second >= radius);
}