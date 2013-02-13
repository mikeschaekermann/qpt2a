#include "StemCellClient.h"
#include "../rendering/RenderManager.h"

void StemCellClient::drawAtTransformation() const
{
	gl::scale(radius, radius, radius);
	RenderManager::getInstance()->renderModel("stemCell", "test",
											  Vec4f(0., 0.5, 0., 0.4),
											  Vec4f(0., 0.3, 0., 0.6),
											  Vec4f(0., 0.9, 0., 0.9),
											  3.);
}