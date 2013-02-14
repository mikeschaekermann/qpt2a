#include "GameObject.h"

GameObject::GameObject(void):
	scale(1, 1, 1)
{}

GameObject::~GameObject(void)
{
	LOG_INFO(concatenate("GameObject deleted. id: ", id));
	
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		(*it)->removeParent(this);
	}

	for (auto it = parents.begin(); it != parents.end(); ++it)
	{
		(*it)->removeChild(this);
	}
}

void GameObject::update(float frameTime)
{
}

void GameObject::addChild(GameObject* child)
{
	children.push_back(child);
};

void GameObject::removeChild(GameObject* child)
{
	auto childInList = find(children.begin(), children.end(), child);

	if (childInList != children.end())
	{
		children.erase(childInList);
	}
};

void GameObject::addParent(GameObject* parent)
{
	parents.push_back(parent);
};

void GameObject::removeParent(GameObject* parent)
{
	auto parentInList = find(parents.begin(), parents.end(), parent);

	if (parentInList != parents.end())
	{
		parents.erase(parentInList);
	}
};

vector<GameObject *> & GameObject::getChildren()
{
	return children;
}