#include "StaticModificatorClient.h"
#include "../rendering/RenderManager.h"

void StaticModificatorClient::drawAtTransformation() const
{
	RENDER_MGR->renderStatic(radius, type, opacity);
}