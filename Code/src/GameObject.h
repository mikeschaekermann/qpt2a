#pragma once

#include <vector>

#include "Client\Config.h"
#include "Player.h"
#include "cinder\Vector.h"
#include "IMotionBehavior.h"
#include "ICollisionBehavior.h"
#include "Client\MessagingBehavior.h"

using namespace cinder;
using namespace ci;

class GameObject
{
public:
	GameObject(Player* player, Vec3f position = Vec3f(0, 0, 0), Vec3f rotation = Vec3f(0, 0, 0), Vec3f scale = Vec3f(1, 1, 1));
	virtual ~GameObject(void);

	Vec3f getPosition() const { return m_position; }
	void setPosition(Vec3f position) { m_position = position; }

	Vec3f getRotation() const { return m_rotation; }
	void setRotation(Vec3f rotation) { m_rotation = rotation; }

	Vec3f getScale() const { return m_scale; }
	void setScale(Vec3f scale) { m_scale = scale; }

	void update(float frameTime);
	void draw() const;

	/**
		@brief adds a child to the list of children
	 */
	void addChild(GameObject* child);

protected:
	/// @brief here the pure draw process is defined, independent from the transformation
	virtual void drawAtTransformation() const;
	
private:
	/// object id assigned by the player class which is unique per player
	int id;
	/// player class which ownes this object
	Player* m_pOwner;
	
	/// current position in the game
	Vec3f m_position;
	/// current rotation of the object
	Vec3f m_rotation;
	/// current scale of the object
	Vec3f m_scale;

	/// all children in the scene graph
	std::vector<GameObject*> m_children;
	/// parent object in the scene graph
	GameObject* m_pParent;

	/// defines the motion of the object
	IMotionBehavior* m_pMotionBehavior;
	/// defines the collision behavior of the object
	ICollisionBehavior* m_pCollisionBehavior;
	/// contains all network messages relevant to this object
	std::vector<MessagingBehavior*> m_messagingBehaviors;
};
