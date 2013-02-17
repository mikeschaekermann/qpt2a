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

	Cam & setPosition(Vec3f newPosition);
	Cam & setFocus(Vec3f newFocus);
	Vec3f screenToWorldPlane(Vec2i screenPoint);

	Vec3f getPosition() const;

private:
	Vec3f position, focus, upDirection;
};

