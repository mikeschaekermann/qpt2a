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

void RenderManager::renderBarrier(float radius)
{
	string barrier = "barrier";
	auto shader = ASSET_MGR->getShaderProg(barrier);

	shader.bind();

	Vec4f ambient = Vec4f(0.2f, 0.2f, 0.22f, 1.f);
	Vec4f diffuse = Vec4f(0.3f, 0.3f, 0.34f, 1.f);
	Vec4f specular = Vec4f(0.f, 0.f, 0.f, 1.f);
	float shininess = 0.f;

	shader.uniform("lightPos", cam.getProjectionMatrix() * cam.getModelViewMatrix() * lightPos);

	shader.uniform("ambientColor", ambient);
	shader.uniform("diffuseColor", diffuse);
	shader.uniform("radius", radius);

	gl::pushModelView();
		drawSphere(Vec3f(0.f, 0.f, 0.f), radius, (int) radius);
	gl::popModelView();

	shader.unbind();
}

void RenderManager::renderStatic(float radius, StaticModificator::Type type, float opacity)
{
	string staticString = "static";
	auto shader = ASSET_MGR->getShaderProg(staticString);

	shader.bind();

	Vec4f ambient; 
	Vec4f diffuse;

	switch (type)
	{
	case StaticModificator::RADIOACTIVITY:
		ambient = Vec4f(0.3f, 0.3f, 0.15f, opacity);
		diffuse = Vec4f(0.75f, 0.6f, 0.12f, opacity);
		break;
	case StaticModificator::NUTRIENTSOIL:
		ambient = Vec4f(0.15f, 0.55f, 0.15f, opacity);
		diffuse = Vec4f(0.14f, 0.4f, 0.14f, opacity);
		break;
	}

	float shininess = 0.f;

	shader.uniform("lightPos", cam.getProjectionMatrix() * cam.getModelViewMatrix() * lightPos);

	shader.uniform("ambientColor", ambient);
	shader.uniform("diffuseColor", diffuse);
	shader.uniform("radius", radius);

	gl::pushModelView();
		drawSphere(Vec3f(0.f, 0.f, 0.f), radius, (int) radius);
	gl::popModelView();

	shader.unbind();
}

void RenderManager::setUp3d()
{
	gl::pushMatrices();
	gl::enableDepthRead();
	gl::setMatrices(cam);
}

void RenderManager::shutdown3d()
{
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