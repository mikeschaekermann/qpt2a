#include "AssetManager.h"

using namespace cinder;

AssetManager* AssetManager::manager = nullptr;

AssetManager::AssetManager(void)
{
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

				tex.setFlipped(false);
				
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

			try
			{
				key = it->getChild("key").getValue();
				MovieGl movie(it->getChild("value").getValue());
				movieMap.insert(pair<string, qtime::MovieGl>(key, movie));
			}
			catch(...)
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
			auto sound = createSound(it->createDoc());
			if(sound)
			{
				soundMap.insert(pair<string, FMOD::Sound*>(it->getChild("key").getValue(), sound));
			}
		}
		else if(it->getTag() == "guiSound")
		{
			auto sound = createSound(*it);
			if(sound)
			{
				guiSoundMap.insert(pair<string, FMOD::Sound*>(it->getChild("key").getValue(), sound));
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
			catch(...)
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
			catch(...)
			{
				LOG_ERROR("Shader file \"" + key + "\" could nod be loaded or linked.\n" + prog.getShaderLog(prog.getHandle()));
			}
		}
	}
}

TriMesh AssetManager::createModel(string filePath)
{
	TriMesh mesh;
	ObjLoader loader(loadFile(filePath));
	loader.load(&mesh);
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
	guiSoundMap.clear();
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