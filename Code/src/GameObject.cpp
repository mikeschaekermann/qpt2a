#include "GameObject.h"
#include "cinder\gl\gl.h"
#include "cinder\app\AppBasic.h"

using namespace cinder;
using namespace gl;

GameObject::GameObject(Player* player, Vec3f position, Vec3f rotation, Vec3f scale) : 
	m_pOwner(player), 
	m_position(position),
	m_rotation(rotation),
	m_scale(scale)
{
}

GameObject::~GameObject(void)
{
#if _DEBUG
	app::console() << "GameObject deleted. id: " << id << std::endl;
#endif

	delete m_pMotionBehavior;
	delete m_pCollisionBehavior;
	
	for(auto it = m_messagingBehaviors.begin(); it != m_messagingBehaviors.end(); ++it)
	{
		delete (*it);
	}
}

void GameObject::update(float frameTime)
{
	m_pMotionBehavior->update(frameTime);
	m_pCollisionBehavior->update(frameTime);

	for(auto it = m_messagingBehaviors.begin(); it != m_messagingBehaviors.end(); ++it)
	{
		(*it)->update(frameTime);
	}

	for(auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->update(frameTime);
	}
}

void GameObject::draw() const
{
	gl::pushMatrices();

	gl::translate(m_position);
	gl::rotate(m_rotation);
	gl::scale(m_scale);

	drawAtTransformation();

	for(auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->draw();
	}

	gl::popMatrices();
}

void GameObject::drawAtTransformation() const
{
	gl::drawSphere(Vec3f(0, 0, 0), 15);
}

void GameObject::addChild(GameObject* child)
{
	if (child != nullptr)
	{
		m_children.push_back(child);
		child->m_pParent = this;
	}
};