#pragma once

#include "../managers/AssetManager.h"
#include "../../common/Config.h"
#include <string>
#include "cinder/Vector.h"
#include "fmod.hpp"
#include "boost/thread/mutex.hpp"

using namespace std;

#define SOUND_PLAYER SoundPlayer::getInstance()

class SoundPlayer
{
public:
	static SoundPlayer * const getInstance();
	static void releaseInstance();

	void setMusicVolume(float volume);
	float getMusicVolume() { return musicVolume; }
	void setSoundVolume(float volume);
	float getSoundVolume() { return soundVolume; }
	

	void playSound(string& key);
	void playSound(string& key, Vec3f pos, Vec3f vel);
	void playMusic(string& key);
	void stopAllSounds();

	void setListener3d(ci::Vec3f pos, ci::Vec3f vel, ci::Vec3f forward, ci::Vec3f up);
	void set3DSettings(float dopplerFact, float distanceFact, float rollOffFact);

	FMOD_RESULT createSound(string& filePath, bool sound3d, bool loop, bool stream, FMOD::Sound** sound);

	void update();

private:
	static SoundPlayer*		instance;
	static boost::mutex		instanceMutex;
	FMOD::System*			system;
	FMOD::Channel*			soundChannels[99];
	FMOD::Channel*			musicChannel;
	unsigned				numSoundChannels,
							nextChannelId;

	FMOD_VECTOR				vPosition,
							vVelocity,
							vForward,
							vUp;

	float					musicVolume,
							soundVolume;

	SoundPlayer(void);
	~SoundPlayer(void);
	SoundPlayer(const SoundPlayer& cpy);
	const SoundPlayer& operator=(const SoundPlayer& cpy);

	void stopAll();
};

