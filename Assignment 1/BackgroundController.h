#pragma once

#include "SFML/Graphics.hpp"
#include "Enums.h"
#include "Renderer.h"
#include "GameObject.h"
#include "SpriteLoader.h"

class BackgroundController : public GameObject
{

	//Methods
public:

	void Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader);
	void Update();
	void SetColour(Color _colour);

	//Variables
private:

	RenderTexture renderTexture;
	Sprite background;
	Texture texture;
	Color backgroundColour = Color::White;

};

