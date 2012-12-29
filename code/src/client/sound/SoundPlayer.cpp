#include "SoundPlayer.h"

SoundPlayer * SoundPlayer::instance = nullptr;

SoundPlayer::SoundPlayer(void) :
	sound3d(false)
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
	}

	return instance;
}

void SoundPlayer::playSound(string& key, bool loop, bool sound3d, bool stream)
{
	system->playSound(FMOD_CHANNEL_FREE, AssetManager::getInstance()->getSound(key), false, &soundChannel);
}

void SoundPlayer::playMusic(string& key, bool loop, bool sound3d, bool stream)
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
	FMOD_VECTOR vPosition = { pos.x, pos.y, pos.z }; 
	FMOD_VECTOR vVelocity = { vel.x, vel.y, vel.z }; 
	FMOD_VECTOR vForward = { forward.x, forward.y, forward.z }; 
	FMOD_VECTOR vUp = { up.x, up.y, up.z }; 
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
	
	return system->createSound(filePath.c_str(),  fmodFlags, NULL, sound);
}