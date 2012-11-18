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
	GameObject(Player* player, Vec3f position, Vec3f rotation);
	virtual ~GameObject(void);

	Vec3f getPosition() const { return position; }
	void setPosition(Vec3f position) { this->position = position; }

	Vec3f getRotation() const { return rotation; }
	void setRotation(Vec3f rotation) { this->rotation = rotation; }

	Vec3f getScale() const { return scale; }
	void setScale(Vec3f scale) { this->scale = scale; }

	void update(float frameTime);
	void draw();

private:
	/// object id assigned by the player class which is unique per player
	int id;
	/// player class which ownes this object
	Player* owner;
	
	/// current position in the game
	Vec3f position;
	/// current rotation of the object
	Vec3f rotation;
	/// current scale of the object
	Vec3f scale;

	/// all children in the scene graph
	std::vector<GameObject*> children;
	/// parent object in the scene graph
	GameObject* parent;

	/// defines the motion of the object
	IMotionBehavior* motionBehavior;
	/// defines the collision behavior of the object
	ICollisionBehavior* collisionBehavior;
	/// contains all network messages relevant to this object
	std::vector<MessagingBehavior*> messagingBehaviors;
};

