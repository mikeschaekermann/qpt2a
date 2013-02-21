#include "AssetManager.h"
#include "../rendering/RenderManager.h"
#include "../../common/ConfigurationDataHandler.h"
#include "../sound/SoundPlayer.h"

using namespace cinder;

AssetManager* AssetManager::manager = nullptr;
boost::mutex AssetManager::instanceMutex;

AssetManager::AssetManager(void)
{
	SOUND_PLAYER->set3DSettings(CONFIG_FLOAT("data.sound.doppler"), CONFIG_FLOAT("data.sound.distance"), CONFIG_FLOAT("data.sound.rollOff"));
}

AssetManager::~AssetManager(void)
{
	clearGameAssets();
	clearGuiAssets();
}

void AssetManager::loadAssets(string filePath)
{
	XmlTree doc(loadFile(filePath));

	for(auto it = doc.begin(); it != doc.end(); ++it)
	{
		if(it->getTag() == "texture")
		{
			string key;

			try
			{
				key = it->getChild("key").getValue();
				textureMap.insert(pair<string, Texture>(key, Texture(loadImage(it->getChild("value").getValue()))));
			}
			catch(...)
			{
				LOG_ERROR("Texture could not be loaded. Texture name: " + key);
			}
		}
		else if(it->getTag() == "guiTexture")
		{
			string key,
				   value;
			Texture tex;
			try
			{
				key = it->getChild("key").getValue();
				value = it->getChild("value").getValue();
				if(value.substr(value.length() - 3) == "svg")
				{
					auto asset = loadFile(value);
					auto doc = svg::Doc::create(asset);

					auto width = atoi(it->getChild("width").getValue().c_str());
					auto height = atoi(it->getChild("height").getValue().c_str());

					cairo::SurfaceImage srf(width, height, true);
					cairo::Context ctx( srf );
					
					auto size = doc->getSize();
					
					if (size.x != 0 && size.y != 0)
					{
						auto scaleMatrix = ci::cairo::Matrix();
						scaleMatrix.initScale((float)width / (float)size.x, (float)height / (float)size.y);
						ctx.setMatrix(scaleMatrix);
					}

					ctx.render(*doc);
					srf.flush();
					tex = gl::Texture(srf.getSurface());
				}
				else
				{
					tex = Texture(loadImage(value));
				}
				
				guiTextureMap.insert(pair<string, Texture>(key, tex));
			}
			catch(...)
			{
				LOG_ERROR("GUI Texture could not be loaded. Texture name: " + key);
			}
		}
		else if(it->getTag() == "movie")
		{
			string key;
			string tmpFilePath;
			try
			{
				key = it->getChild("key").getValue();
				auto value = it->getChild("value").getValue();

				const unsigned BUFSIZE = 4096;

				TCHAR* relPath = new TCHAR[value.size() + 1];
				std::copy(value.begin(), value.end(), relPath);
				TCHAR buffer[BUFSIZE] = TEXT("");
				TCHAR** lppPart = {NULL};

				GetFullPathName(relPath,
					BUFSIZE,
					buffer,
					lppPart);

				MovieGl movie(buffer);
				
				movieMap.insert(pair<string, qtime::MovieGl>(key, movie));
			}
			catch(exception& e)
			{
				LOG_ERROR("Movie could not be loaded. Movie name: " + key);
			}
		}
		else if(it->getTag() == "guiMovie")
		{
			string key;

			try
			{
				key = it->getChild("key").getValue();
				MovieGl movie(it->getChild("value").getValue());
				guiMovieMap.insert(pair<string, qtime::MovieGl>(key, movie));
			}
			catch(...)
			{
				LOG_ERROR("GUI Movie could not be loaded. Movie name: " + key);
			}
		}
		else if(it->getTag() == "sound")
		{
			auto sound = createSound(*it);
			if(sound)
			{
				soundMap.insert(pair<string, FMOD::Sound*>(it->getChild("key").getValue(), sound));
			}
		}
		else if(it->getTag() == "model")
		{
			TriMesh model;
			string key;

			try
			{
				key = it->getChild("key").getValue();
				model = createModel(it->getChild("value").getValue());
				modelMap.insert(pair<string, TriMesh>(key, model));	
			}
			catch(exception)
			{
				LOG_ERROR("Model could not be loaded. Model name: " + it->getChild("key").getValue());
			}
		}
		else if(it->getTag() == "shader")
		{
			GlslProg prog;
			string key;

			try
			{
				key = it->getChild("key").getValue();
				auto vertShader = readShaderFile(it->getChild("vertexShader").getValue());
				auto fragShader = readShaderFile(it->getChild("fragmentShader").getValue());
				prog = GlslProg(vertShader.c_str(), fragShader.c_str());

				shaderMap.insert(pair<string, GlslProg>(key, prog));
			}
			catch(exception& ex)
			{
				LOG_ERROR("Shader file: " + key);
				LOG_ERROR("Exception: " + string(ex.what()));
			}
		}
	}
}

TriMesh AssetManager::createModel(string filePath)
{
	TriMesh mesh;
	ObjLoader loader(loadFile(filePath));;
	loader.load(&mesh, boost::logic::tribool::true_value, boost::logic::tribool::false_value);
	return mesh;
}

string AssetManager::readShaderFile(string filePath)
{
	ifstream stream(filePath);
	return string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
}

void AssetManager::clearGameAssets()
{
	soundMap.clear();
	textureMap.clear();
	movieMap.clear();
	shaderMap.clear();
	modelMap.clear();
}

void AssetManager::clearGuiAssets()
{
	guiMovieMap.clear();
	guiTextureMap.clear();
}

FMOD::Sound* AssetManager::createSound(XmlTree xml)
{
	string filePath = xml.getChild("value").getValue();
	bool sound3d = (xml.getChild("3d").getValue() == "true" ? true : false);
	bool loop = (xml.getChild("loop").getValue() == "true" ? true : false);
	bool stream = (xml.getChild("stream").getValue() == "true" ? true : false);
	
	FMOD::Sound* sound;
	auto result = SoundPlayer::getInstance()->createSound(filePath, sound3d, loop, stream, &sound);
	
	if (result != FMOD_OK)
	{
		LOG_ERROR("SoundPlayer: Could not load sound file " + filePath);
		return nullptr;
	}
	else
	{
		return sound;
	}
}

TriMesh const & AssetManager::getModel(string& modelName) const
{
	try
	{
		return modelMap.at(modelName);
	}
	catch(std::exception& ex)
	{
		manager->assetErrorOutput(ex, modelName, "getModel");
		static TriMesh const & defaultResult = TriMesh();
		return defaultResult;
	}
}

Texture const & AssetManager::getTexture(string& textureName) const
{
	try
	{
		return textureMap.at(textureName);
	}
	catch(std::exception& ex)
	{
		manager->assetErrorOutput(ex, textureName, "getTexture");
	}
}

Texture const & AssetManager::getGuiTexture(string& textureName) const
{
	try
	{
		return guiTextureMap.at(textureName);
	}
	catch(std::exception& ex)
	{
		manager->assetErrorOutput(ex, textureName, "getGuiTexture");
	}
}
	
MovieGl const & AssetManager::getMovie(string& movieName) const
{
	try
	{
		return movieMap.at(movieName);
	}
	catch(std::exception& ex)
	{
		manager->assetErrorOutput(ex, movieName, "getMovie");
	}
}

MovieGl const & AssetManager::getGuiMovie(string& movieName) const
{
	try
	{
		return guiMovieMap.at(movieName);
	}
	catch(std::exception& ex)
	{
		manager->assetErrorOutput(ex, movieName, "getGuiMovie");
	}
}
	
FMOD::Sound * AssetManager::getSound(string& soundName) const
{
	try
	{
		return soundMap.at(soundName);
	}
	catch(std::exception& ex)
	{
		manager->assetErrorOutput(ex, soundName, "getSound");
	}
}

GlslProg const & AssetManager::getShaderProg(string& shaderName) const
{
	try
	{
		return shaderMap.at(shaderName);
	}
	catch(std::exception& ex)
	{
		manager->assetErrorOutput(ex, shaderName, "getShaderProg");
	}
}

void AssetManager::assetErrorOutput(exception& ex, string& assetName, string methodName) const
{
	LOG_WARNING("Message: " + string(ex.what()) + "\n" +
				"Asset name: " + assetName + "\n" +
				"Method name: " + methodName);
}