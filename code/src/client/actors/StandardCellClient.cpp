#include "StandardCellClient.h"

void StandardCellClient::drawAtTransformation() const
{
	gl::color(0, 0, 1);
	drawSphere(Vec3f(0, 0, 0), radius);
}