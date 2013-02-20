#include "CellClient.h"
#include "PolypeptideClient.h"
#include "../rendering/RenderManager.h"

PolypeptideClient::PolypeptideClient():
	focusCenter(Vec3f::zero()),
	focusRadius(0.f)
{}

void PolypeptideClient::drawAtTransformation() const
{
	gl::pushMatrices();
		auto scale = CONFIG_FLOAT("data.polypeptide.scale");
		gl::scale(scale, scale, scale);
		RENDER_MGR->renderBlackShadedModel("poly");
	gl::popMatrices();
}

void PolypeptideClient::setCenterOfFocus(Vec3f center)
{
	focusCenter = center;
}

void PolypeptideClient::setRadiusOfFocus(float radius)
{
	focusRadius = radius;
}

void PolypeptideClient::setFocus(Vec3f center, float radius)
{
	setCenterOfFocus(center);
	setRadiusOfFocus(radius);
}