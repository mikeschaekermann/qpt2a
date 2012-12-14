#pragma once

#include "../common/Config.h"
#include "ICollisionBehavior.h"
#include "cinder/gl/gl.h"

#include <vector>

using namespace gl;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject(void);

	unsigned getId() const { return m_uiId; }
	void setId(unsigned id) { m_uiId = id; }

	Vec3f getPosition() const { return m_position; }
	void setPosition(Vec3f position) { m_position = position; }

	Vec3f getRotation() const { return m_rotation; }
	void setRotation(Vec3f rotation) { m_rotation = rotation; }

	Vec3f getScale() const { return m_scale; }
	void setScale(Vec3f scale) { m_scale = scale; }

	void setCollisionBehavior(ICollisionBehavior* collisionBehavior) { m_pCollisionBehavior = collisionBehavior; }

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
	/// current position in the game
	Vec3f m_position;
	/// current rotation of the object
	Vec3f m_rotation;
	/// current scale of the object
	Vec3f m_scale;

private:
	/// object id assigned by the player class which is unique per player
	unsigned m_uiId;
	
	/// all children in the scene graph
	vector<GameObject*> m_children;
	/// all parents in the scene graph
	vector<GameObject*> m_parents;

	/// game object's collision behavior
	ICollisionBehavior* m_pCollisionBehavior;
};