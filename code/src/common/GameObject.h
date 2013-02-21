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

	virtual void update(float frameTime);

	void addChild(GameObject* child);
	void removeChild(GameObject* child);

	void addParent(GameObject* parent);
	void removeParent(GameObject* parent);

	bool wantsToBeDestroyed() const { return mayBeDeleted; }

	vector<GameObject *> & getChildren();

protected:
	/// flag indicating whether this object may be deleted
	/// to allow self-destruction during for-loops
	bool mayBeDeleted;
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
};