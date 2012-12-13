#pragma once

#include "cinder/Camera.h"
#include "cinder/Vector.h"

using namespace cinder;

class Cam :
	public cinder::CameraPersp
{
public:
	Cam();
	Cam(int width, int height, float fov, float nearPlane, float farPlane);
	virtual ~Cam(void);

	Vec3f screenToWorld(Vec2i screenPoint);

private:
	Vec3f unproject(Vec3f point);
};

