// MenuController.cpp : This class acts as a basic menu controller

#include "MenuController.h"
#include "Game.h"

void MenuButton::Initialise(GameRenderer* _gameRenderer, string _text, Game* _game, int _x, int _y, TextController* _textController)
{

	//Set pointers
	gameRenderer = _gameRenderer;
	game = _game;
	textController = _textController;

	//Load the font
	if (!font.loadFromFile("Aquarius-RegularMono.ttf"))
	{
		toolSet.ConsoleOut("Failed to load font!");
		exit(1);
	}

	//Set the font
	text.setFont(font);

	//Set string and text paramaters
	text.setString(_text);
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setFillColor(Color(255, 241, 232));
	
	//Set up the rendertexture
	renderTexture.create(text.getGlobalBounds().width + 10, text.getGlobalBounds().height + 10);

	//Set text position based on the above paramaters
	text.setPosition(renderTexture.getSize().x/2, renderTexture.getSize().y/2);

	//Set position
	x = _x;
	y = _y;

}

void MenuButton::Update()
{

	//Draw button to render texture
	renderTexture.clear(Color::Transparent);
	renderTexture.draw(text);

	//Convert button to texture and display with sprite
	const Texture& texture = renderTexture.getTexture();

	sprite.setTexture(texture);
	sprite.setPosition(x, y);

	//Check for input
	Vector2f mousePos = gameRenderer->window.mapPixelToCoords(Mouse::getPosition());

	//Adjust target sprite scale if the mouse is contained in the bounding box 
	if (sprite.getGlobalBounds().contains(mousePos))
	{

		scaleTarget = 2;

		//If mouse button is pressed, execute the given method
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			Execute();
		}

	}
	else
	{
		scaleTarget = 1;
	}

	if (scale > scaleTarget)scale -= 0.1;
	if (scale < scaleTarget)scale += 0.1;

	//Scale the sprite, using -scale for y to prevent it being upside down, and adjust the origin to match.
	sprite.setScale(scale, -scale);
	sprite.setOrigin(sprite.getLocalBounds().width / 2, abs(sprite.getLocalBounds().height / 2));

	gameRenderer->AddDrawable(DEPTH_GUI_MAX + 100, sprite);

}

void StartButton::Execute()
{

	//Switch to game state and load new room
	textController->Dump();
	game->gameState = GAME;
	game->roomController->NewMap();
	game->roomController->LoadRoom();

}

void QuitButton::Execute()
{
	//End game
	exit(0);
}

void CreditsButton::Execute()
{
	
	//If the text controller is empty
	if (textController->IsEmpty())
	{

		textController->AddNewTextBox("A game by: Definitely A Real Game Company Inc.");
		textController->AddNewTextBox("Lead programming: Joris Bohnson");
		textController->AddNewTextBox("Lead artist: Mheresa Tay");
		textController->AddNewTextBox("Programming by: Cavid Dameron & Bony Tlair");
		textController->AddNewTextBox("Art by: Targaret Mhatcher & Cames Jallaghan");
		textController->AddNewTextBox("Actual credits: Various utility function code snippets from Stack Overflow (praise be the copy and paste)");
		textController->AddNewTextBox("Alarm sound effect taken from Open Game Art (opengameart.org) under the Public Domain CC0 license");
		textController->AddNewTextBox("Aquarius Regular by Floodfonts (1001fonts.com)");

	}

}

void PauseButton::Execute()
{
	//Resume game
	game->pauseState = 0;
}

void QuitToMenuButton::Execute()
{
	//Reset the game
	game->ResetGame();
}


void MenuController::Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader, Game* _game, TextController* _textController = nullptr)
{

	//Set up pointers
	game = _game;
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;
	textController = _textController;

	//Set up main menu buttons
	StartButton* startButton = new StartButton();
	startButton->Initialise(gameRenderer, "Start", game, 120, 135, textController);

	QuitButton* quitButton = new QuitButton();
	quitButton->Initialise(gameRenderer, "Quit", game, 120, 265);

	CreditsButton* creditsButton = new CreditsButton();
	creditsButton->Initialise(gameRenderer, "Credits", game, 120, 200, textController);

	//Push to the vector
	buttons.push_back(startButton);
	buttons.push_back(creditsButton);
	buttons.push_back(quitButton);

	//Set up pause menu buttons
	PauseButton* resumeButton = new PauseButton();
	resumeButton->Initialise(gameRenderer, "Resume", game, 120, 135, textController);

	QuitToMenuButton* quitToMenuButton = new QuitToMenuButton();
	quitToMenuButton->Initialise(gameRenderer, "Quit To Menu", game, 120, 200);

	//Push to the vector
	pauseButtons.push_back(resumeButton);
	pauseButtons.push_back(quitToMenuButton);

	//Set up the sprite
	sprites = spriteLoader->GetTexture("spritesheet.png");

	IntRect rect(46, 25, 77, 51);

	logo.setTextureRect(rect);
	logo.setTexture(sprites);

	logo.setOrigin(logo.getLocalBounds().width / 2, logo.getLocalBounds().height / 2);

}

void MenuController::Update()
{

	logoRotation += 0.1;
	logo.setScale(3, 3);
	logo.setPosition(120, -50);
	logo.setRotation(sin(logoRotation) * logoRotationIntensity);

	gameRenderer->AddDrawable(DEPTH_GUI_MAX, logo);

	//Update buttons
	switch (menuState)
	{
		case 0:

			for (MenuButton* button : buttons)
			{
				button->Update();
			}

		break;

		case 1:

			for (MenuButton* button : pauseButtons)
			{
				button->Update();
			}

		break;
	}

}