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
											  Vec4f(0.f, 0.4f, 0.f, 0.6f),
											  Vec4f(0.f, 0.2f, 0.f, 0.3f),
											  Vec4f(0.f, 0.9f, 0.f, 0.9f),
											  4.f);

	gl::popMatrices();

	gl::translate(0.f, 0.f, - radius * 0.3f);

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
													  Vec4f(0.1f, 0.1f, 0.1f, 1.0f),
													  Vec4f(0.1f, 0.1f, 0.1f, 1.0f),
													  Vec4f(0.9f, 0.9f, 0.9f, 1.0f),
													  10.f);

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