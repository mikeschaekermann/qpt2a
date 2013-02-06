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
					cairo::SurfaceImage srf(atoi(it->getChild("width").getValue().c_str()), atoi(it->getChild("height").getValue().c_str()), true);
					cairo::Context ctx( srf );
					
					auto boundingBox = doc->getBoundingBox();
					auto b1 = doc->getBoundingBoxAbsolute();
					auto b2 = doc->getBounds();
					auto s = doc->getSize();
					auto ding = doc->getWidth();
					auto dang = doc->getHeight();
					auto rootNode = doc->findNode("svg", false);
					auto t = rootNode->getTransform();

					auto inverseMatrix = ctx.getMatrix();
					inverseMatrix.invert();
					
					

					ctx.setMatrix(inverseMatrix);
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
			ModelBufferData model;
			string key;

			try
			{
				key = it->getChild("key").getValue();
				model = createModel(it->getChild("value").getValue());
				modelMap.insert(pair<string, ModelBufferData>(key, model));	
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

ModelBufferData AssetManager::createModel(string filePath)
{
	/// import model
	Assimp::Importer imp;
	/// this tells assimp to remove point & line primitives, so that
	/// triangles remain as the only primitive type
	imp.SetPropertyInteger( AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE );
	/// this tells assimp to scale the model so that all vertices lie in
	/// the [-1, 1] range
	imp.SetPropertyInteger( AI_CONFIG_PP_PTV_NORMALIZE , 1 );
	/// for an overview of all postprocessing flags, see
	/// http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	const aiScene* scene = imp.ReadFile( filePath,
		aiProcess_PreTransformVertices | aiProcess_JoinIdenticalVertices |
		aiProcess_FindDegenerates | aiProcess_SortByPType |
		aiProcess_Triangulate | aiProcess_RemoveComponent |
		aiProcess_ValidateDataStructure | aiProcess_GenSmoothNormals | aiProcess_FixInfacingNormals);

	/// import has failed
	if (scene == nullptr)
	{
		cout << "Could not import the file: " << filePath << endl;
		throw new string("Geht nid!");
	}

	/// read vertex- and index-values from the imported mesh structure
	vector<aiVector3D> vertices;
	vector<unsigned> indices;
	vector<aiVector3D> normals;

	aiMesh* mesh;

	for (unsigned m = 0; m < scene->mNumMeshes; ++m)
	{
		mesh = scene->mMeshes[m];

		for (unsigned v= 0; v < mesh->mNumVertices; ++v)
		{
			vertices.push_back(mesh->mVertices[v]);
			normals.push_back(mesh->mNormals[v]);
		}

		for (unsigned f = 0; f < mesh->mNumFaces; ++f)
		{
			for (unsigned i = 0; i < mesh->mFaces[f].mNumIndices; ++i)
			{
				indices.push_back(mesh->mFaces[f].mIndices[i]);
			}
		}
	}

	unsigned indicesCounter = indices.size();

	GLuint indexBufferObject;
	GLuint vertexBufferObject;
	GLuint normalBufferObject;

	/// create an index buffer object
	glGenBuffers( 1, &indexBufferObject );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferObject );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW );

	/// create a vertex buffer object
	glGenBuffers( 1, &vertexBufferObject );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBufferObject );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( aiVector3D ), &vertices[0], GL_STATIC_DRAW );

	/// create a vertex normal buffer object
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(aiVector3D), &normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return ModelBufferData(normalBufferObject, vertexBufferObject, indexBufferObject, indicesCounter);
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