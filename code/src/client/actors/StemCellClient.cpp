#include "StemCellClient.h"

void StemCellClient::drawAtTransformation() const
{
	gl::color(ColorA(0, 1, 0, opacity));
	drawSphere(Vec3f(0, 0, 0), radius);
}