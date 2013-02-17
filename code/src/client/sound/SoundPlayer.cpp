#include "SoundPlayer.h"

SoundPlayer * SoundPlayer::instance = nullptr;

SoundPlayer::SoundPlayer(void)
{
}

SoundPlayer::~SoundPlayer(void)
{
}

SoundPlayer * const SoundPlayer::getInstance()
{
	if(!instance)
	{
		instance = new SoundPlayer();
		auto result = FMOD::System_Create(&(instance->system));
		
		if (result != FMOD_OK)
		{
			LOG_ERROR(concatenate("FMOD error! FMOD result: ", result));
		}
		
		unsigned version;
		instance->system->getVersion(&version);
		if (version < FMOD_VERSION)
		{
			LOG_WARNING(concatenate("Error! You are using an old version of FMOD", version) +
						concatenate(". This program requires ", FMOD_VERSION));
		}

		instance->system->init(2, 0, nullptr);
	}

	return instance;
}

void SoundPlayer::playSound(string& key)
{
	auto sound = AssetManager::getInstance()->getSound(key);
	system->playSound(FMOD_CHANNEL_FREE, sound, false, &soundChannel);
}

void SoundPlayer::playSound(string& key, Vec3f pos, Vec3f vel)
{
	auto sound = AssetManager::getInstance()->getSound(key);
	system->playSound(FMOD_CHANNEL_FREE, sound, true, &soundChannel);
	FMOD_MODE mode;
	sound->getMode(&mode);
	if(mode ^ FMOD_3D == FMOD_3D)
	{
		FMOD_VECTOR fPos,
					fVel;

		fPos.x = pos.x;
		fPos.y = pos.y;
		fPos.z = pos.z;

		fVel.x = 0; //vel.x;
		fVel.y = 0; //vel.y;
		fVel.z = 0; //vel.z;

		soundChannel->set3DAttributes(&fPos, &fVel);
	}

	soundChannel->setPaused(false);
}

void SoundPlayer::playMusic(string& key)
{
	system->playSound(FMOD_CHANNEL_FREE, AssetManager::getInstance()->getSound(key), false, &musicChannel);
}

void SoundPlayer::update()
{
	system->update();
}

void SoundPlayer::releaseInstance()
{
	stopAll();
	system->close();
	delete instance;
}

void SoundPlayer::stopAll()
{
	soundChannel->stop();
	musicChannel->stop();
}

void SoundPlayer::setMusicVolume(float volume)
{
	bool paused = false;
	musicChannel->getPaused(&paused);
	
	musicChannel->setPaused(true);
	musicChannel->setVolume(volume);
	musicChannel->setPaused(paused);
	
	musicVolume = volume;
}

void SoundPlayer::setSoundVolume(float volume)
{
	bool paused = false;
	soundChannel->getPaused(&paused);

	soundChannel->setPaused(true);
	soundChannel->setVolume(volume);
	soundChannel->setPaused(paused);
	
	soundVolume = volume;
}

void SoundPlayer::setListener3d(ci::Vec3f pos, ci::Vec3f vel, ci::Vec3f forward, ci::Vec3f up)
{
	vPosition.x = pos.x;
	vPosition.y = pos.y;
	vPosition.z = pos.z;

	vVelocity.x = vel.x;
	vVelocity.y = vel.y;
	vVelocity.z = vel.z;

	vForward.x = forward.x;
	vForward.y = forward.y;
	vForward.z = forward.z;

	vUp.x = up.x;
	vUp.y = up.y;
	vUp.z = up.z;

	system->set3DListenerAttributes( 0, &vPosition, &vVelocity, &vForward, &vUp);
}

void SoundPlayer::set3DSettings(float dopplerFact, float distanceFact, float rollOffFact)
{
	system->set3DSettings(dopplerFact, distanceFact, rollOffFact); // doppler scale, distance factor, roll off for exagerating the effect
}	

FMOD_RESULT SoundPlayer::createSound(string& filePath, bool sound3d, bool loop, bool stream, FMOD::Sound** sound)
{
	int fmodFlags;
	if(sound3d)
	{
		fmodFlags = FMOD_3D;
	}
	else
	{
		fmodFlags = FMOD_SOFTWARE;
	}

	if(stream)
	{
		fmodFlags =  fmodFlags | FMOD_CREATESTREAM;
	}
	
	if(loop)
	{
		fmodFlags = fmodFlags | FMOD_LOOP_NORMAL;
	}
	else
	{
		fmodFlags = fmodFlags | FMOD_LOOP_OFF;
	}

	return system->createSound(filePath.c_str(), fmodFlags, NULL, sound);
}