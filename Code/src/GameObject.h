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
	GameObject(Player* player);
	virtual ~GameObject(void);

	Vec3f getPosition() { return position; }
	void setPosition(Vec3f position) { this->position = position; }

	Vec3f getRotation() { return rotation; }
	void setRotation(Vec3f rotation) { this->rotation = rotation; }

	Vec3f getScale() { return scale; }
	void setScale(Vec3f scale) { this->scale = scale; }

	void update(float frameTime);
	void draw();

private:
	int id;
	Player* owner;
	
	Vec3f position;
	Vec3f rotation;
	Vec3f scale;

	std::vector<GameObject*> children;
	GameObject* parent;

	IMotionBehavior* motionBehavior;
	ICollisionBehavior* collisionBehavior;
	std::vector<MessagingBehavior*> messagingBehaviors;
};

