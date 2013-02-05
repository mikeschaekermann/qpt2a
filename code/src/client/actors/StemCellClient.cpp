#include "StemCellClient.h"

void StemCellClient::drawAtTransformation() const
{
	gl::color(0, 1, 0);
	drawSphere(Vec3f(0, 0, 0), radius);
}