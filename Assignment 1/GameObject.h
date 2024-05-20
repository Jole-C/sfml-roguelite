#pragma once
#include <SFML/Graphics.hpp>
#include "Tools.h"
#include "Renderer.h"
#include "SpriteLoader.h"

using namespace sf;

class GameObject
{

	//Methods
public:

private:

	//Variables
public:

	double x = 0;
	double y = 0;
	double animationFrame;
	int depth;

	Tools toolSet;
	Texture sprites;
	GameRenderer* gameRenderer;
	SpriteLoader* spriteLoader;

private:
};

