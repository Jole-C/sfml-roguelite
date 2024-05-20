//Game.cpp: The core of the program. Responsible for initialisation of the game, updating, and drawing. Also responsible for passing references, for example, from the player controller to the enemy controller.
//

#include "Game.h"

//Initialisation section. Responsible for initialising all game components.
//
void Game::Initialise()
{

	//initialise the sprite loader
	spriteLoader->LoadTextureFromImage("tileset.png");
	spriteLoader->LoadTextureFromImage("spritesheet.png");
	spriteLoader->LoadTextureFromImage("overlay.png");

	//Initialise the game renderer
	gameRenderer->Initialise(1920, 1080);

	//Initialise the room controller
	roomController->Initialise(gameRenderer, spriteLoader);

	//Initialise the text controller
	textController->Initialise(gameRenderer);

	//Seed the game
	srand((unsigned)time(NULL));
	
	//Initialise player
	playerController->Initialise(gameRenderer, roomController, spriteLoader, 512, 512);

	//Initialise the enemy controller and give the roomcontroller a pointer to it
	//This is done here as the room controller MUST be initialised where it is as the
	//playercontroller needs access to it
	//The playercontroller gets access first as the enemy does not need to spawn enemies in
	//the start room
	enemyController->Initialise(gameRenderer, spriteLoader, playerController, roomController, textController, backgroundController);
	roomController->enemyController = enemyController;

	//Initialise the menu controller
	menuController->Initialise(gameRenderer, spriteLoader, this, textController);

	//Initialise the background controller
	backgroundController->Initialise(gameRenderer, spriteLoader);

	//Initialise the ending controller
	endingController->Initialise(gameRenderer, spriteLoader, textController);

	//Give the player controller a pointer to the enemy controller
	playerController->enemyController = enemyController;

}

//Update section. Responsible for updating all game components.
//
void Game::Update()
{

	//Background
	if (gameState == GAME || gameState == GAME_MENU)
	{
		backgroundController->Update();
	}

	//Switch statement for different game states
	switch (gameState)
	{
		case GAME_INTRO:
		{

			if (startIntro == true)
			{
				startIntro = false;
				textController->AddNewTextBox("In the year 23XX, where people were still too lazy to think up an extra 2 digits, a new threat is brewing on the edge of the solar system... Mysterious aliens have appeared, and threaten earth with their mere existence even though they didn't really do anything wrong until we attacked first. Now, it's up to you to stop them.");
				textController->AddNewTextBox("Hey captain, how come it's only me going into the alien's secret base?");
				textController->AddNewTextBox("No idea.");
				textController->AddNewTextBox("Oh.");
				textController->AddNewTextBox("Press Escape to continue... If you dare. Ooooooooooooooooooooooooooooooooooo.....");
			}

			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				textController->Dump();
				startIntro = true;
				gameState = GAME_MENU;
			}

			break;
		}
		case GAME_MENU:
		{

			//Update the menu
			backgroundController->SetColour(Color::White);
			menuController->Update();

			break;

		}
		case GAME:
		{

			//Pause state switching
			switch (pauseState)
			{
				case 0:

					menuController->menuState = 0;

					if (Keyboard::isKeyPressed(Keyboard::Escape))
					{
						pauseState = 1;
						menuController->menuState = 1;
						gameRenderer->view.setCenter(120, 67);
					}

					//Update objects
					roomController->Update();
					enemyController->Update();
					playerController->Update();

					if (playerController->deathCooldown <= 0)
					{
						ResetGame();
					}

					//Ending sequence
					if (roomController->EscapeRoomReached() == true)
					{
						gameState = GAME_WON;
					}

				break;

				case 1:

					menuController->menuState = 1;
					menuController->Update();

				break;
			}

			break;

		}
		case GAME_WON:
		{

			endingController->Update();

			break;
		}
		case GAME_LOST:
		{
			break;
		}

	}
	toolSet.ConsoleOut(to_string(pauseState));

	//Ending reset
	if (endingController->endingReset == true)
	{
		ResetGame();
	}

	//Update the text controller
	textController->Update();

}

//Draw section. Responsible for drawing all game components.
//
void Game::Draw()
{
	
	//Update the view
	gameRenderer->UpdateView();

	//Clear the previous frame
	gameRenderer->Clear();

	//Render all objects stored in the renderqueue and display the screen and the current frame
	gameRenderer->Display();

	//Clear drawables
	if (playerController->deathCooldown > 100)
	{
		gameRenderer->ClearDrawables();
	}

	//Set the framerate
	gameRenderer->SetFrameRate(60);

}

void Game::Run()
{
	while (gameRenderer->IsOpen())
	{

		gameRenderer->Poll();

		Update();
		Draw();

	}
}

void Game::ResetGame()
{

	//Reset game state
	gameState = GAME_MENU;

	//Reset all game components
	enemyController->Reset();
	playerController->Reset();
	roomController->Reset();
	endingController->Reset();
	menuController->menuState = 0;
	gameRenderer->view.setCenter(120, 67);

	//Dump all text out of the controller
	textController->Dump();

}