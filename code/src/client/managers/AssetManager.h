#pragma once

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
#include "cinder/ObjLoader.h"
#include "cinder/TriMesh.h"

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

	TriMesh const & getModel(string& modelName) const;
	
	Texture const & getTexture(string& textureName) const;
	Texture const & getGuiTexture(string& textureName) const;
	
	MovieGl const & getMovie(string& movieName) const;
	MovieGl const & getGuiMovie(string& movieName) const;
	
	FMOD::Sound * getSound(string& soundName) const;
	FMOD::Sound * getGuiSound(string& soundName) const;
	
	GlslProg const & getShaderProg(string& shaderName) const;
	
	void clearGuiAssets();
	void clearGameAssets();
	void releaseInstance() { delete manager; }

private:
	static AssetManager*							manager;

	unordered_map<string, TriMesh>					modelMap;
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

	TriMesh createModel(string filePath);
	string readShaderFile(string filePath);
	FMOD::Sound* createSound(XmlTree xml);
	void assetErrorOutput(exception& ex, string& assetName, string methodName) const;
};