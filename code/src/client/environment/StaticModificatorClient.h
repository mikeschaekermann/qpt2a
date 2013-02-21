#pragma once

#include "../actors/GameObjectClient.h"
#include "../../common/environment/StaticModificator.h"
#include "../sound/SoundPlayer.h"

class StaticModificatorClient : virtual public GameObjectClient, virtual public StaticModificator
{
public:
	StaticModificatorClient(unsigned id, Vec3f position, Vec3f rotation, Vec3f scale, 
		float radius, StaticModificator::Type type) :
		GameObjectClient(), StaticModificator(id, position, rotation, scale, radius, type)
	{
		opacity = .3f;
		if(type == StaticModificator::Type::RADIOACTIVITY)
		{
			SOUND_PLAYER->playSound(string("radioactive"), position, Vec3f::zero());
		}
		else if(type == StaticModificator::Type::NUTRIENTSOIL)
		{
			SOUND_PLAYER->playSound(string("nutriensoil"), position, Vec3f::zero());
		}
	}

protected:
	void drawAtTransformation() const;
private:
	virtual void setId(unsigned int id) { GameObjectClient::setId(id); }
	virtual void setPosition(Vec3f position) { StaticModificator::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { StaticModificator::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { StaticModificator::setScale(scale); }
	virtual void setRadius(float radius) { StaticModificator::setRadius(radius); }
};