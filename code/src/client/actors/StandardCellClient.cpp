#include "StandardCellClient.h"

void StandardCellClient::startAnimation()
{
	/// animation
}

void StandardCellClient::drawAtTransformation() const
{
	gl::color(ColorA(0, 0, 1, opacity));
	drawSphere(Vec3f(0, 0, 0), radius);
}