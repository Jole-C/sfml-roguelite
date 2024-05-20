#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Textures
{
public:
	Texture texture;
	string textureName;
};

class SpriteLoader
{

	//Methods
public:

	Texture LoadTextureFromImage(string _directory); //Core method - Loads and returns a new texture, storing it in a list for future reference
	Texture GetTexture(string _textureName); //Core method - Returns a texture from the texture sheet
	

private:



	//Variables
public:



private:

	vector<Textures> textureList; //Holds all textures

};

