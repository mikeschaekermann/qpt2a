#include "RenderManager.h"

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager()
{
	auto screenSize = getWindowSize();
	cam = Cam(screenSize.x, screenSize.y,
			  CONFIG_FLOAT1("data.rendering.camera.fieldOfView"),
			  CONFIG_FLOAT1("data.rendering.camera.nearPlane"),
			  CONFIG_FLOAT1("data.rendering.camera.farPlane"));

	cam
		.setPosition(Vec3f( 0.0f, 0.0f, 0.0f ))
		.setFocus(Vec3f::zero());

	gl::enableAlphaBlending();
}

RenderManager * const RenderManager::getInstance()
{
	if(instance == nullptr)
	{
		instance = new RenderManager();
	}

	return instance;
}

void RenderManager::renderModel(string modelName,
							   string shaderName,
							   Vec4f ambient,
							   Vec4f diffuse,
							   Vec4f specular,
							   float shininess)
{
	auto model = ASSET_MGR->getModel(modelName);
	auto shader = ASSET_MGR->getShaderProg(shaderName);

	shader.bind();

	shader.uniform("lightPos", cam.getProjectionMatrix() * cam.getModelViewMatrix() * lightPos);

	shader.uniform("ambientColor", ambient);
	shader.uniform("diffuseColor", diffuse);
	shader.uniform("specularColor", specular);
	shader.uniform("shininess", shininess);

	gl::pushModelView();
		gl::draw(model);
	gl::popModelView();

	shader.unbind();
}

void RenderManager::setUp3d()
{
	gl::pushMatrices();
		gl::enableDepthWrite();
		gl::enableDepthRead();

		gl::setMatrices(cam);
}

void RenderManager::shutdown3d()
{
		gl::disableDepthWrite();
		gl::disableDepthRead();
	gl::popMatrices();
}

void RenderManager::zoomToWorld()
{
	/// world radius = sin(camera's fov / 2) * camera's distance
	/// camera's distance = world radius / sin(camera's fov / 2)
	float worldRadius = CONFIG_FLOAT1("data.world.radius");
	float camDistance = worldRadius / (float) sin(cam.getFov() / 2.f / 180.f * M_PI);

	cam.setPosition(Vec3f(0, 0, camDistance)).setFocus(Vec3f::zero());
}