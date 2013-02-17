#include "StaticModificatorClient.h"
#include "../rendering/RenderManager.h"

void StaticModificatorClient::drawAtTransformation() const
{
	gl::color(ColorA(0.8f, 0.8f, 0.2f, opacity));
	gl::drawSphere(Vec3f(), radius, radius);
	//RENDER_MGR->renderStatic(radius, type, opacity);
}