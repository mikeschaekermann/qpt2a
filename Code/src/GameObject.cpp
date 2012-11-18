#include "GameObject.h"
#include "cinder\gl\gl.h"
#include "cinder\app\AppBasic.h"

using namespace cinder;
using namespace gl;

GameObject::GameObject(Player* player, Vec3f position, Vec3f rotation) : 
	owner(player), 
	position(position),
	rotation(rotation),
	scale(Vec3f(1, 1, 1))
{
}

GameObject::~GameObject(void)
{
#if _DEBUG
	app::console() << "GameObject deleted. id: " << id << std::endl;
#endif

	delete motionBehavior;
	delete collisionBehavior;
	
	for(auto it = messagingBehaviors.begin(); it != messagingBehaviors.end(); ++it)
	{
		delete (*it);
	}
}

void GameObject::update(float frameTime)
{
	motionBehavior->update(frameTime);
	collisionBehavior->update(frameTime);

	for(auto it = messagingBehaviors.begin(); it != messagingBehaviors.end(); ++it)
	{
		(*it)->update(frameTime);
	}
}

void GameObject::draw()
{
	drawSphere(position, 10);
}