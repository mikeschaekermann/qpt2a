#pragma once

#include "../managers/AssetManager.h"
#include "Cam.h"
#include "../../common/ConfigurationDataHandler.h"
#include "../environment/StaticModificatorClient.h"
#include "MarchingCubes.h"

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
	Cam cam;

	static RenderManager * const getInstance();
	static void releaseInstance() { if(instance) delete instance; }

	void renderSlimeShadedModel(string modelName,
					 Vec3f worldTranslation = Vec3f(0, 0, 0),
					 Vec4f ambient = Vec4f(0.1f, 0.1f, 0.1f, 1.f),
					 Vec4f diffuse = Vec4f(0.4f, 0.4f, 0.4f, 1.f),
					 Vec4f specular = Vec4f(1.f, 1.f, 1.f, 1.f),
					 float shininess = 10.f);
	
	void renderSlimeShadedModel(TriMesh model,
					 Vec3f worldTranslation = Vec3f(0, 0, 0),
					 Vec4f ambient = Vec4f(0.1f, 0.1f, 0.1f, 1.f),
					 Vec4f diffuse = Vec4f(0.4f, 0.4f, 0.4f, 1.f),
					 Vec4f specular = Vec4f(1.f, 1.f, 1.f, 1.f),
					 float shininess = 10.f);
	
	void renderPhongShadedModel(string modelName,
					 Vec4f ambient = Vec4f(0.1f, 0.1f, 0.1f, 1.f),
					 Vec4f diffuse = Vec4f(0.4f, 0.4f, 0.4f, 1.f),
					 Vec4f specular = Vec4f(1.f, 1.f, 1.f, 1.f),
					 float shininess = 10.f);
	
	void renderPhongShadedModel(TriMesh model,
					 Vec4f ambient = Vec4f(0.1f, 0.1f, 0.1f, 1.f),
					 Vec4f diffuse = Vec4f(0.4f, 0.4f, 0.4f, 1.f),
					 Vec4f specular = Vec4f(1.f, 1.f, 1.f, 1.f),
					 float shininess = 10.f);

	void renderBarrier(float radius);

	void renderStatic(float radius, StaticModificator::Type type, float opacity);

	void setUp3d();
	void shutdown3d();

	void zoomToWorld();

private:
	Vec3f lightPos;

	static RenderManager* instance;

	float fogOfWarOpacity;
	float fogOfWarInnerRadius;
	float fogOfWarOuterRadius;

	TriMesh fogOfWarLayer;

	RenderManager(void);
	RenderManager(const RenderManager&) {}
	~RenderManager();

	void initializeFogOfWar();
	void drawFogOfWar() const;
};

