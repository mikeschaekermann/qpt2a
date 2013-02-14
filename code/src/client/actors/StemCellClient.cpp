#include "StemCellClient.h"
#include "../rendering/RenderManager.h"

void StemCellClient::drawAtTransformation() const
{
	auto h = getHue();
	auto s = getSaturation();
	auto b = getBrightness();
	auto o = getOpacity();

	ColorA ambientColor(CM_HSV, h, s * 0.4, b * 0.4, o * 0.4);
	ColorA diffuseColor(CM_HSV, h, s * 0.6, b * 0.6, o * 0.6);
	ColorA specularColor(CM_HSV, h, s, b, o);

	gl::scale(radius, radius, radius);
	RenderManager::getInstance()->renderModel("stemCell", "test",
											  Vec4f(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a),
											  Vec4f(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a),
											  Vec4f(specularColor.r, specularColor.g, specularColor.b, specularColor.a),
											  3.);
}