#include "Cam.h"
#include "cinder/app/AppBasic.h"

using namespace cinder::app;

Cam::Cam()
{

}

Cam::Cam(int width, int height, float fov, float nearPlane, float farPlane) : CameraPersp(width, height, fov, nearPlane, farPlane)
{
}

Cam::~Cam(void)
{
}

Vec3f Cam::screenToWorld(Vec2i screenPoint)
{
	// Find near and far plane intersections
	Vec3f point3f = Vec3f((float)screenPoint.x, getWindowSize().y * 0.5f - (float)screenPoint.y, 0.0f);
	Vec3f nearPlane = unproject(point3f);
	Vec3f farPlane = unproject(Vec3f(point3f.x, point3f.y, 1.0f));

	// Calculate X, Y and return point
	float theta = (0.0f - nearPlane.z) / (farPlane.z - nearPlane.z);
	return Vec3f(nearPlane.x + theta * (farPlane.x - nearPlane.x), nearPlane.y + theta * (farPlane.y - nearPlane.y), 0.0f);
}

Vec3f Cam::unproject(Vec3f point)
{
	// Find the inverse Modelview-Projection-Matrix
	Matrix44f invMVP = mProjectionMatrix * mModelViewMatrix;
	invMVP.invert();

	// Transform to normalized coordinates in the range [-1, 1]
	Vec4f pointNormal;
	auto viewport = getWindowSize();
	pointNormal.x = (point.x - viewport.x) / viewport.x * 2.0f - 1.0f;
	pointNormal.y = (point.y - viewport.y) / viewport.y * 2.0f;
	pointNormal.z = 2.0f * point.z - 1.0f;
	pointNormal.w = 1.0f;

	// Find the object's coordinates
	Vec4f pointCoord = invMVP * pointNormal;
	if (pointCoord.w != 0.0f)
		pointCoord.w = 1.0f / pointCoord.w;

	// Return coordinate
	return Vec3f(pointCoord.x * pointCoord.w, pointCoord.y * pointCoord.w, pointCoord.z * pointCoord.w);
}