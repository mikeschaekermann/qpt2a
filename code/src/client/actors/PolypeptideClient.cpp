#include "CellClient.h"
#include "PolypeptideClient.h"
#include "../rendering/RenderManager.h"

void PolypeptideClient::drawAtTransformation() const
{
	gl::pushMatrices();
		auto scale = CONFIG_FLOAT1("data.polypeptide.scale");
		gl::scale(scale, scale, scale);
		RENDER_MGR->renderBlackShadedModel("poly");
	gl::popMatrices();
}