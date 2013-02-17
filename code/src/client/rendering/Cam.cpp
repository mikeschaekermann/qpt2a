#include "Cam.h"
#include "cinder/app/AppBasic.h"
#include "../../common/Config.h"
#include "../../common/ConfigurationDataHandler.h"

using namespace cinder::app;

Cam::Cam()
{

}

Cam::Cam(int width, int height, float fov, float nearPlane, float farPlane) :
	CameraPersp(width, height, fov, nearPlane, farPlane),
	upDirection(Vec3f::zAxis())
{
}

Cam::~Cam(void)
{
}

Cam & Cam::setPosition(Vec3f newPosition)
{
	position = newPosition;

	auto minDistance = max<float>(1.1 * getNearClip(), CONFIG_FLOAT2("data.rendering.camera.distance.min", 100));
	auto maxDistance = min<float>(0.9 * getFarClip(), CONFIG_FLOAT2("data.rendering.camera.distance.max", 1000));

	position.z = min<float>(max<float>(position.z, minDistance), maxDistance);

	float horizontalAngle = ci::toRadians(CONFIG_FLOAT2("data.rendering.camera.angle.horizontal", 45));
	float verticalAngle = ci::toRadians(CONFIG_FLOAT2("data.rendering.camera.angle.vertical", 10));

	focus.x = position.x + cos(horizontalAngle) * sin(verticalAngle) * position.z;
	focus.y = position.y + sin(horizontalAngle) * sin(verticalAngle) * position.z;
	focus.z = 0;

	lookAt(position, focus, upDirection);

	return *this;
}

Vec3f Cam::getPosition() const
{
	return position;
}

Cam & Cam::setFocus(Vec3f newFocus)
{
	focus = newFocus;
	lookAt(position, focus, upDirection);

	return *this;
}

Vec3f Cam::screenToWorldPlane(Vec2i screenPoint)
{
	float u = (float)screenPoint.x / (float)getWindowWidth();
	float v = 1.f - ((float) screenPoint.y / (float) getWindowHeight());

	auto ray = generateRay(u, v, getAspectRatio());

	// z-coordinates of both cam position and gazing direction
	// must be non-zero and must have inverse signs
	// so that camera looks onto the world plane (i.e. plane with z == 0) from a distance
	if (ray.getOrigin().z * ray.getDirection().z >= 0)
	{
		throw exception("Cannot calculate the coordinates on the world plane from screen coordinates, because the camera does not look onto the world plane from a distance.");
	}

	// normalize x and y against z (same as stretching direction vector so that z == 1),
	// stretch it against the z-distance of the camera
	// and add camera's x- and y-offsets to the coordinates
	// to get x and y coordinates on the 2D-plane with z == 0, i.e. the world plane
	float x = ray.getDirection().x / abs(ray.getDirection().z) * ray.getOrigin().z + ray.getOrigin().x;
	float y = ray.getDirection().y / abs(ray.getDirection().z) * ray.getOrigin().z + ray.getOrigin().y;

	return Vec3f(x, y, 0);
}