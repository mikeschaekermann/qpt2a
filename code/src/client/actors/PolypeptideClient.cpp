#include "CellClient.h"
#include "PolypeptideClient.h"
#include "../rendering/RenderManager.h"

void PolypeptideClient::drawAtTransformation()
{
	RENDER_MGR->renderBlackShadedModel("poly");
}