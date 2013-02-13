#pragma once

#include "../managers/AssetManager.h"
#include "Cam.h"
#include "../../common/ConfigurationDataHandler.h"

#include <string>

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::gl;

class RenderManager
{
public:
	Vec3f lightPos;
	Cam cam;

	static RenderManager * const getInstance();
	static void releaseInstance() { if(instance) delete instance; }

	void renderModel(string modelName,
					 string shaderName,
					 Vec4f ambient = Vec4f(0.1, 0.1, 0.1, 1.),
					 Vec4f diffuse = Vec4f(0.4, 0.4, 0.4, 1.),
					 Vec4f specular = Vec4f(1., 1., 1., 1.),
					 float shininess = 10.);

	void setUp3d();
	void shutdown3d();

	void zoomToWorld();

private:
	static RenderManager* instance;

	RenderManager(void);
	RenderManager(const RenderManager&) {}
	~RenderManager(){}
};

