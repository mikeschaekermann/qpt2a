#pragma once

#include <vector>

#include "../common/Config.h"


class GameObject
{
public:
	virtual ~GameObject(void);

	unsigned getId() const { return id; }
	virtual void setId(unsigned int id) { this->id = id; }
	
	Vec3f getPosition() const { return position; }
	virtual void setPosition(Vec3f position) { this->position = position; }

	Vec3f getRotation() const { return rotation; }
	virtual void setRotation(Vec3f rotation) { this->rotation = rotation; }

	Vec3f getScale() const { return scale; }
	virtual void setScale(Vec3f scale) { this->scale = scale; }

	float getRadius() const { return radius; }
	virtual void setRadius(float radius) { this->radius = radius; }

	void update(float frameTime);

	/**
		@brief adds a child to the list of children
		@param child		pointer to child to be added
	 */
	void addChild(GameObject* child);
	/**
		@brief adds a parent to the list of parents
		@param parent		pointer to parent to be added
	 */
	void addParent(GameObject* parent);

protected:
	/// unique id identifying the game object within the process
	unsigned int id;
	/// current position in the game
	Vec3f position;
	/// current rotation of the object
	Vec3f rotation;
	/// current scale of the object
	Vec3f scale;
	/// game object's radius
	float radius;

	GameObject(void);
private:
	/// all children in the scene graph
	vector<GameObject*> children;
	/// all parents in the scene graph
	vector<GameObject*> parents;
	
	GameObject(const GameObject & cpy);
};