#include "BoneCellClient.h"
#include "../rendering/RenderManager.h"
#include "../managers/GameManager.h"
#include <cmath>

void BoneCellClient::drawAtTransformation() const
{
	gl::pushMatrices();

	auto h = getHue();
	auto s = getSaturation();
	auto b = getBrightnessFromHealthPoints();
	auto o = getOpacityFromHealthPoints();

	ColorA ambientColor(CM_HSV, h, s * 0.4f, b * 0.4f, o * 0.4f);
	ColorA diffuseColor(CM_HSV, h, s * 0.6f, b * 0.6f, o * 0.6f);
	ColorA specularColor(CM_HSV, h, s, b, o);

	gl::scale(radius, radius, radius);
	RenderManager::getInstance()->renderSlimeShadedModel("boneCell",
											  position,
											  Vec4f(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a),
											  Vec4f(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a),
											  Vec4f(specularColor.r, specularColor.g, specularColor.b, specularColor.a),
											  4.f);

	gl::popMatrices();
}

void BoneCellClient::increaseGlobalTypeCounter() const
{
	++(POLYCAPACITY->NumberOfBoneCells);
}

void BoneCellClient::decreaseGlobalTypeCounter() const
{
	if (POLYCAPACITY->NumberOfBoneCells == 0)
	{
		LOG_ERROR("Tried to remove a bone cell although the bone cell counter is already zero.");
		assert(false);
	}
	else
	{
		--(POLYCAPACITY->NumberOfBoneCells);
	}
}
