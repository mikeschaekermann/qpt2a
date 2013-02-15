#include "BarrierClient.h"
#include "../rendering/RenderManager.h"

void BarrierClient::drawAtTransformation() const
{
	gl::color(ColorA(0.3f, 0.3f, 0.5f, opacity));

	RENDER_MGR->renderBarrier(radius);
}