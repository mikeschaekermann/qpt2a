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

	gl::scale(radius, radius, radius);

	RenderManager::getInstance()->renderModel("standardCell", "test",
											  Vec4f(0., 0.4, 0., 0.6),
											  Vec4f(0., 0.2, 0., 0.3),
											  Vec4f(0., 0.9, 0., 0.9),
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