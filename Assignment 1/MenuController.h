#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameObject.h"
#include "Renderer.h"
#include "SpriteLoader.h"
#include "TextController.h"
#include "Enums.h"
#include "Tools.h"

class Game;

//Holds button info
class MenuButton : public GameObject
{

	//Methods
public:

	void Initialise(GameRenderer* _gameRenderer, string _text, Game* _game, int _x, int _y, TextController* _textController = nullptr);
	void Update();
	virtual void Execute() { toolSet.ConsoleOut("Base output of button upon click. Should not happen."); }; //The code to execute

	//Variables
public:

	Text text;
	Font font;

	Sprite sprite;
	RenderTexture renderTexture;
	TextController* textController;
	Game* game;

	double scale = 1;
	int scaleTarget = 1;

};

class StartButton : public MenuButton
{
	//Methods
public:

	void Execute();

};

class CreditsButton : public MenuButton
{
	//Methods
public:

	void Execute();
};

class QuitButton : public MenuButton
{
	//Methods
public:

	void Execute();

};

class PauseButton : public MenuButton
{
	//Methods
public:

	void Execute();
};

class QuitToMenuButton : public MenuButton
{
	//Methods
public:

	void Execute();

};

class MenuController : public GameObject
{

	//Methods
public:

	void Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader, Game* _game, TextController* _textController);
	void Update();

	//Variables
public:

	GameRenderer* gameRenderer;
	SpriteLoader* spriteLoader;
	TextController* textController;
	Game* game;

	vector<MenuButton*> buttons;
	vector<MenuButton*> pauseButtons;

	int menuState = 0; //0 = main menu, 1 = pause menu
	
	Sprite logo;

	double logoRotation = 0;
	double logoRotationIntensity = 3;

};