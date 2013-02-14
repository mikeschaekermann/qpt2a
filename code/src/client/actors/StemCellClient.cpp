#include "StemCellClient.h"
#include "../rendering/RenderManager.h"

void StemCellClient::drawAtTransformation() const
{
	gl::scale(radius, radius, radius);
	RenderManager::getInstance()->renderModel("stemCell", "test",
											  Vec4f(0.f, 0.5f, 0.f, 0.4f),
											  Vec4f(0.f, 0.3f, 0.f, 0.6f),
											  Vec4f(0.f, 0.9f, 0.f, 0.9f),
											  3.f);
}