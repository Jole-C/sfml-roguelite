#pragma once

#include "Enums.h"
#include "Renderer.h"
#include "RoomController.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "TextController.h"
#include "MenuController.h"
#include "BackgroundController.h"
#include "EndingController.h"
#include "Tools.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Game
{

	//methods
public:
	void Initialise(); //Initialise all game components
	void Update(); //Updates all game components
	void Draw(); //Draws all game components
	void Run(); //Handles updating and drawing for combination of polling events
	void ResetGame(); //Resets to default state

private:


	//variables
public:
	GAME_STATES gameState = GAME_INTRO; //Sets the state of the game for state machine

	//Screen size
	int screenWidth = 1920;
	int screenHeight = 1080;

	//Display
	GameRenderer* gameRenderer = new GameRenderer;
	SpriteLoader* spriteLoader = new SpriteLoader;
	RenderStates states;

	//Room controller
	RoomController* roomController = new RoomController;

	//Player
	PlayerController* playerController = new PlayerController;

	//Enemies
	EnemyController* enemyController = new EnemyController;

	//Text
	TextController* textController = new TextController;

	//Menu
	MenuController* menuController = new MenuController;

	//Background
	BackgroundController* backgroundController = new BackgroundController;

	//Ending
	EndingController* endingController = new EndingController;
	
	//Clock for rough FPS
	float fps;
	sf::Clock clock = sf::Clock::Clock();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

	//Intro
	bool startIntro = true;

	//Pause
	int pauseState = 0;

	//Toolset
	Tools toolSet;


private:

};

