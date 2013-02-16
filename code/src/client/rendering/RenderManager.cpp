#include "RenderManager.h"
#include "boost/thread/thread.hpp"
#include "../../common/ConfigurationDataHandler.h"
#include "../managers/GameManager.h"

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager():
	run(true),
	fogOfWarOpacity(CONFIG_FLOAT2("data.world.fogOfWar.opacity", 0.9)),
	fogOfWarInnerRadius(CONFIG_FLOAT2("data.world.fogOfWar.innerRadius", 20)),
	fogOfWarOuterRadius(CONFIG_FLOAT2("data.world.fogOfWar.outerRadius", 20)),
	fogOfWarSurfaceFront(new Surface())
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

	fogOfWarLayer.appendVertex(Vec3f(-1.f,  1.f, -1.f)); /// top left
	fogOfWarLayer.appendVertex(Vec3f( 1.f,  1.f, -1.f)); /// top right
	fogOfWarLayer.appendVertex(Vec3f(-1.f, -1.f, -1.f)); /// bottom left
	fogOfWarLayer.appendVertex(Vec3f( 1.f, -1.f, -1.f)); /// bottom right

	fogOfWarLayer.appendTriangle(0, 2, 1);
	fogOfWarLayer.appendTriangle(1, 2, 3);

	//fogOfWarThread = boost::thread(boost::bind(&RenderManager::updateFogOfWar, this));
}

RenderManager::RenderManager(const RenderManager&):
	run(true),
	fogOfWarOpacity(CONFIG_FLOAT2("data.world.fogOfWar.opacity", 0.9)),
	fogOfWarInnerRadius(CONFIG_FLOAT2("data.world.fogOfWar.innerRadius", 20)),
	fogOfWarOuterRadius(CONFIG_FLOAT2("data.world.fogOfWar.outerRadius", 20)),
	fogOfWarSurfaceFront(new Surface())
{
}

RenderManager::~RenderManager()
{
	run = false;

	fogOfWarThread.join();
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

void RenderManager::setUp3d()
{
	gl::pushMatrices();
		gl::enableDepthWrite();
		gl::enableDepthRead();

		gl::setMatrices(cam);
}

void RenderManager::shutdown3d()
{
		auto fogOfWarShaderProgram = ASSET_MGR->getShaderProg(string("fogOfWar"));
		fogOfWarShaderProgram.bind();

		GAME_SCR.getContainerMutex().lock();

		auto& cellsExploring = GAME_SCR.getExploringCells();
		int numOfCells = cellsExploring.getSize();

		Vec2f * centers2D = new Vec2f[numOfCells];
		Vec2f * radii2D = new Vec2f[numOfCells];
		
		unsigned int i = 0;

		for (auto it = cellsExploring.begin(); it != cellsExploring.end(); ++it)
		{
			auto center3D = it->second->getPosition();
			auto center2D = cam.worldToScreen(center3D, getWindowWidth(), getWindowHeight());
			
			auto radius3D = it->second->getRadius();
			auto innerRadius2D = center2D.distance(cam.worldToScreen(center3D + Vec3f(radius3D + fogOfWarInnerRadius, 0, 0), getWindowWidth(), getWindowHeight()));
			auto outerRadius2D = center2D.distance(cam.worldToScreen(center3D + Vec3f(radius3D + fogOfWarInnerRadius + fogOfWarOuterRadius, 0, 0), getWindowWidth(), getWindowHeight()));

			centers2D[i] = center2D;
			radii2D[i].x = innerRadius2D;
			radii2D[i].y = outerRadius2D;

			++i;
		}

		fogOfWarShaderProgram.uniform(string("screenSize"), getWindowSize());
		fogOfWarShaderProgram.uniform(string("numOfCells"), numOfCells);
		fogOfWarShaderProgram.uniform(string("centers2D"), centers2D, numOfCells);
		fogOfWarShaderProgram.uniform(string("radii2D"), radii2D, numOfCells);

		GAME_SCR.getContainerMutex().unlock();

		gl::draw(fogOfWarLayer);

		fogOfWarShaderProgram.unbind();

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

void RenderManager::generateFogOfWarTexture()
{
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWindowWidth(), getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	
	float border[4] = {1.f, 1.f, 1.f, 1.f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_RGBA, GL_TEXTURE_2D, textureHandle, 0);

	GLenum result = glCheckFramebufferStatus (GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != result) {
		cout << "ERROR: Framebuffer is not complete.\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderManager::updateFogOfWar()
{
	while (run)
	{
		fogOfWarMutex.lock();

		fogOfWarLayer = TriMesh();
		Vec3f topLeft, topRight, bottomLeft, bottomRight;
		cam.getNearClipCoordinates(&topLeft, &topRight, &bottomLeft, &bottomRight);

		fogOfWarLayer.appendVertex(topLeft);
		fogOfWarLayer.appendVertex(topRight);
		fogOfWarLayer.appendVertex(bottomLeft);
		fogOfWarLayer.appendVertex(bottomRight);

		fogOfWarLayer.appendColorRGBA(ColorA(1.f, 0.f, 0.f, 0.5f));
		fogOfWarLayer.appendColorRGBA(ColorA(1.f, 0.f, 0.f, 0.5f));
		fogOfWarLayer.appendColorRGBA(ColorA(1.f, 0.f, 0.f, 0.5f));
		fogOfWarLayer.appendColorRGBA(ColorA(1.f, 0.f, 0.f, 0.5f));

		fogOfWarLayer.appendTriangle(0, 2, 1);
		fogOfWarLayer.appendTriangle(1, 2, 3);

		fogOfWarMutex.unlock();

		/*
		GAME_SCR.getContainerMutex().lock();

		auto& cellsExploring = GAME_SCR.getExploringCells();
		
		vector<Vec2f> positions;
		vector<float> innerRadii;
		vector<float> outerRadii;
		
		positions.reserve(cellsExploring.getSize());
		innerRadii.reserve(cellsExploring.getSize());
		outerRadii.reserve(cellsExploring.getSize());

		for (auto it = cellsExploring.begin(); it != cellsExploring.end(); ++it)
		{
			auto center3D = it->second->getPosition();
			auto center2D = cam.worldToScreen(center3D, getWindowWidth(), getWindowHeight());
			
			auto radius3D = it->second->getRadius();
			auto innerRadius2D = center2D.distance(cam.worldToScreen(center3D + Vec3f(radius3D + fogOfWarInnerRadius, 0, 0), getWindowWidth(), getWindowHeight()));
			auto outerRadius2D = center2D.distance(cam.worldToScreen(center3D + Vec3f(radius3D + fogOfWarInnerRadius + fogOfWarOuterRadius, 0, 0), getWindowWidth(), getWindowHeight()));
			
			positions.emplace_back(center2D);
			innerRadii.emplace_back(innerRadius2D);
			outerRadii.emplace_back(outerRadius2D);
		}

		GAME_SCR.getContainerMutex().unlock();
		*/
		/*
		for (auto it = positionsAndRadii.begin(); it != positionsAndRadii.end(); ++it)
		{
			auto innerRadiusSq = innerRadius * innerRadius;
			auto outerRadiusSq = outerRadius * outerRadius;
			auto radiusDifferenceSq = outerRadiusSq - innerRadiusSq;
			/// multiplication by 1.1 to avoid sharp
			/// horizontal and vertical edges
			auto halfSize = Vec2f(outerRadius * 1.1, outerRadius * 1.1);
			auto reverseHalfSize = Vec2f(outerRadius * 1.1, - outerRadius * 1.1);
			auto upperLeft = center2D - halfSize;
			auto lowerRight = center2D + halfSize;
			auto upperRight = center2D - reverseHalfSize;
			auto lowerLeft = center2D + reverseHalfSize;

			auto circleArea = Area(upperLeft, lowerRight);
		
			if (
				getWindowBounds().contains(upperLeft) ||
				getWindowBounds().contains(lowerRight) ||
				getWindowBounds().contains(upperRight) ||
				getWindowBounds().contains(lowerLeft)
			)
			{
				auto pixel = fogOfWarSurfaceBack->getIter();

				while( pixel.line() )
				{
					while( pixel.pixel() )
					{
						auto distanceSq = pixel.getPos().distanceSquared(center2D);
						if (distanceSq <= innerRadiusSq)
						{
							pixel.a() = 0.f;
						}
						else if (distanceSq <= outerRadiusSq)
						{
							auto opacity = (distanceSq - innerRadiusSq) / radiusDifferenceSq * 255.f;
							pixel.a() = min<float>(opacity, pixel.a());
						}
					}
				}
			}
		}
		

		fogOfWarMutex.lock();

		auto tmpSurface = fogOfWarSurfaceFront;
		fogOfWarSurfaceFront = fogOfWarSurfaceBack;
		delete tmpSurface;

		fogOfWarMutex.unlock();
		*/
	}
}

void RenderManager::shiftFogOfWar(Vec2f shift)
{
	auto fogOfWarSurfaceBack = new Surface(getWindowWidth(), getWindowHeight(), false, cinder::SurfaceChannelOrder::RGBA);

	auto it = fogOfWarSurfaceBack->getIter();

	while( it.line() )
	{
		while( it.pixel() )
		{
			it.r() = it.g() = it.b() = 0.f;
			it.a() = 255.f;
		}
	}

	fogOfWarMutex.lock();

	fogOfWarSurfaceBack->copyFrom(*fogOfWarSurfaceFront, fogOfWarSurfaceFront->getBounds(), shift);
	
	auto tmpSurface = fogOfWarSurfaceFront;
	fogOfWarSurfaceFront = fogOfWarSurfaceBack;
	delete tmpSurface;

	fogOfWarMutex.unlock();
}

void RenderManager::drawFogOfWar()
{
	gl::color(ColorA(1, 1, 1, 0.9));

	fogOfWarMutex.lock();

	gl::draw(gl::Texture(*fogOfWarSurfaceFront), getWindowBounds());

	fogOfWarMutex.unlock();
}
