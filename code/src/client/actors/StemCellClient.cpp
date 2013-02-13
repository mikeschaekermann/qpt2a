#include "StemCellClient.h"
#include "../managers/GameManager.h"

void StemCellClient::drawAtTransformation() const
{
	gl::color(ColorA(0, 1, 0, opacity));
	GAME_SCR.renderModel("stemcell", "test", Vec3f(100, 0, 100));
}