#pragma once
#include "Enums.h"
#include "GameObject.h"
#include "SpriteLoader.h"
#include "ParticleController.h"
#include "TextController.h"
#include "Renderer.h"


class EndingController : public GameObject
{

	//Methods
public:

	void Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader, TextController* _textController);
	void Update();
	void Reset();

	//Variables
public:

	ENDING_STATES endingState = ENDING_SHIPFLYOFF;

	ParticleController particleController;
	TextController* textController;

	//sprites
	Sprite evilBase;
	Sprite goodShip;
	Sprite evilShip;

	Vector2f goodShipPos = Vector2f(120, 135);
	Vector2f evilShipPos = Vector2f(-600, 50);
	Vector2f evilBasePos = Vector2f(120, 135);

	//frames
	int sceneFrames = 500;

	//Misc variables
	double evilBaseRotation;
	bool endingReset = false;

};

