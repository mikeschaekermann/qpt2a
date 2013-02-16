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

#define RENDER_MGR RenderManager::getInstance()

class RenderManager
{
public:
	Vec3f lightPos;
	Cam cam;

	static RenderManager * const getInstance();
	static void releaseInstance() { if(instance) delete instance; }

	void renderModel(string modelName,
					 string shaderName,
					 Vec4f ambient = Vec4f(0.1f, 0.1f, 0.1f, 1.f),
					 Vec4f diffuse = Vec4f(0.4f, 0.4f, 0.4f, 1.f),
					 Vec4f specular = Vec4f(1.f, 1.f, 1.f, 1.f),
					 float shininess = 10.f);

	void renderBarrier(float radius);

	void setUp3d();
	void shutdown3d();

	void zoomToWorld();

	void shiftFogOfWar(Vec2f shift);
	void drawFogOfWar();

private:
	static RenderManager* instance;

	/// flag indicating whether threads can be closed or not
	volatile bool run;

	/// fog-of-war information
	Surface * fogOfWarSurfaceFront;
	const float fogOfWarOpacity;
	const float fogOfWarInnerRadius;
	const float fogOfWarOuterRadius;

	boost::thread fogOfWarThread;
	boost::mutex fogOfWarMutex;

	TriMesh fogOfWarLayer;

	RenderManager(void);
	RenderManager(const RenderManager&);
	~RenderManager();

	void updateFogOfWar();
};

