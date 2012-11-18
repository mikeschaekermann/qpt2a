#include "GameObject.h"
#include "cinder\gl\gl.h"

using namespace cinder;
using namespace gl;

GameObject::GameObject(Player* player) : owner(player)
{
}

GameObject::~GameObject(void)
{
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