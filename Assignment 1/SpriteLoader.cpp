///SpriteLoader.cpp : This class acts as an easy way to store and access all textures.

#include "SpriteLoader.h"


Texture SpriteLoader::LoadTextureFromImage(string _directory)
{

	Textures texture;
	Texture tex = texture.texture;

	if (!tex.loadFromFile(_directory))
	{
		cout << "Error loading Texture: " << _directory;
		exit(1);
	}

	texture.texture = tex;
	texture.textureName = _directory;

	textureList.push_back(texture);

	return tex;

}

Texture SpriteLoader::GetTexture(string _textureName)
{

	bool couldntLoad = true;

	for(Textures texture : textureList)
	{
		if (texture.textureName == _textureName)
		{
			return texture.texture;
			couldntLoad = false;
		}
	}

	if (couldntLoad)
	{
		cout << "Could not find a Texture matching the name: " << _textureName;
	}

}