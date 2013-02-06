#pragma once

#include "../assets/ModelBufferData.h"
#include "../sound/SoundPlayer.h"
#include "../../common/Config.h"

#include <unordered_map>
#include <string>

#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/Xml.h"
#include "cinder/ImageIo.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/svg/Svg.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/svg/Svg.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Timeline.h"

#include "fmod.hpp"

#include "ASSIMP/Importer.hpp"
#include "ASSIMP/config.h"
#include "ASSIMP/postprocess.h"
#include "ASSIMP/scene.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace cinder::gl;
using namespace cinder::qtime;

#define ASSET_MGR AssetManager::getInstance()

class AssetManager
{
public:
	~AssetManager(void);

	static AssetManager * const getInstance() { return (manager != nullptr ? manager : (manager = new AssetManager())); }
	void loadAssets(string filePath);

	ModelBufferData const & getModel(string& modelName) const { return modelMap.at(modelName); }
	
	Texture const & getTexture(string& textureName) const { return textureMap.at(textureName); }
	Texture const & getGuiTexture(string& textureName) const { return guiTextureMap.at(textureName); }
	
	MovieGl const & getMovie(string& movieName) const { return movieMap.at(movieName); }
	MovieGl const & getGuiMovie(string& movieName) const { return guiMovieMap.at(movieName); }
	
	FMOD::Sound * getSound(string& soundName) const { return soundMap.at(soundName); }
	FMOD::Sound * getGuiSound(string& soundName) const { return guiSoundMap.at(soundName); }
	
	GlslProg const & getShaderProg(string& shaderName) const { return shaderMap.at(shaderName); }
	
	void clearGuiAssets();
	void clearGameAssets();
	void releaseInstance() { delete manager; }

private:
	static AssetManager*							manager;

	unordered_map<string, ModelBufferData>			modelMap;
	unordered_map<string, Texture>					textureMap;
	unordered_map<string, Texture>					guiTextureMap;
	unordered_map<string, MovieGl>					movieMap;
	unordered_map<string, MovieGl>					guiMovieMap;
	unordered_map<string, FMOD::Sound*>				soundMap;
	unordered_map<string, FMOD::Sound*>				guiSoundMap;
	unordered_map<string, GlslProg>					shaderMap;

	AssetManager(void);
	AssetManager(const AssetManager& manager);
	const AssetManager& operator=(const AssetManager& manager);

	ModelBufferData createModel(string filePath);
	string readShaderFile(string filePath);
	FMOD::Sound* createSound(XmlTree xml);
};