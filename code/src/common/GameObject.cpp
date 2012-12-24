#include "GameObject.h"

GameObject::GameObject(void) { }

GameObject::~GameObject(void)
{
	//LOG_INFO(concatenate("GameObject deleted. id: ", id));
}

void GameObject::update(float frameTime)
{
	for(auto it = children.begin(); it != children.end(); ++it)
	{
		(*it)->update(frameTime);
	}
}

void GameObject::addChild(GameObject* child)
{
	children.push_back(child);
	child->addParent(this);
};

void GameObject::addParent(GameObject* parent)
{
	parents.push_back(parent);
};