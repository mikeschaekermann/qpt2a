#include "StandardCellClient.h"
#include "../rendering/RenderManager.h"

void StandardCellClient::drawAtTransformation() const
{
	gl::scale(radius, radius, radius);
	RenderManager::getInstance()->renderModel("standardCell", "test",
											  Vec4f(0., 0.4, 0., 0.6),
											  Vec4f(0., 0.2, 0., 0.3),
											  Vec4f(0., 0.9, 0., 0.9),
											  4.);
}