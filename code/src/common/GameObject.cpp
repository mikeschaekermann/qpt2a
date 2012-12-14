#include "GameObject.h"

GameObject::GameObject(unsigned id) : 
	m_uiId(id),
	m_position(0, 0, 0),
	m_rotation(0, 0, 0),
	m_scale(1, 1, 1)
{
}

GameObject::~GameObject(void)
{
	LOG_INFO(concatenate("GameObject deleted. id: ", m_uiId));
}

void GameObject::update(float frameTime)
{
	for(auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->update(frameTime);
	}
}

void GameObject::addChild(GameObject* child)
{
	m_children.push_back(child);
	child->addParent(this);
};

void GameObject::addParent(GameObject* parent)
{
	m_parents.push_back(parent);
};