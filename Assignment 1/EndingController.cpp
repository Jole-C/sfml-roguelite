#include "EndingController.h"


void EndingController::Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader, TextController* _textController)
{

	//Initialise pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;
	textController = _textController;

	//Set up the sprites
	sprites = spriteLoader->GetTexture("spritesheet.png");

	//Good ship sprite
	goodShip.setTexture(sprites);
	goodShip.setTextureRect(IntRect(293, 35, 27, 19));
	goodShip.setScale(3, 3);

	//Evil ship sprite
	evilShip.setTexture(sprites);
	evilShip.setTextureRect(IntRect(256, 38, 33, 20));
	evilShip.setScale(-7, 7);

	//Evil base sprite
	evilBase.setTexture(sprites);
	evilBase.setTextureRect(IntRect(42, 83, 102, 58));
	evilBase.setScale(7, 7);
	evilBase.setOrigin(evilBase.getLocalBounds().width / 2, evilBase.getLocalBounds().height / 2);

	//Set camera position
	gameRenderer->ViewSetFollowCoordinates(120, 67);

	//Set up particle controller
	particleController.Initialise(1000, gameRenderer, spriteLoader);

}

void EndingController::Update()
{

	//Do ending animations
	switch (endingState)
	{
	case ENDING_SHIPFLYOFF:

		//Add initial text and disable alarm
		if (sceneFrames == 500)
		{

			//Add initial text
			textController->AddNewTextBox("We did it! We escaped!");
			textController->AddNewTextBox("And we absolutely DEFINITELY destroyed the mainframe and stopped the alien's secret weapon.");

			//Stop alarm
			gameRenderer->ToggleAlarmOverlay();

			gameRenderer->view.setCenter(120, 64);

		}

		//Explosion particles
		for (int i = 0; i < 1; i++)
		{

			//Ship thrust
			particleController.AddParticle(goodShipPos.x - 10, goodShipPos.y + 20, 180, 0.01, 1, toolSet.RandomRange(1, 3), 0.95, toolSet.RandomRange(60, 120), Color(255, 163, 0));
			particleController.AddParticle(goodShipPos.x - 10, goodShipPos.y + 20, 180, 0.01, 1, toolSet.RandomRange(1, 3), 0.95, toolSet.RandomRange(60, 120), Color(95, 87, 79));

			//Fire
			particleController.AddParticle(toolSet.RandomRange(-500, 800), 300, 270, toolSet.RandomRange(3, 6), 0.97, toolSet.RandomRange(3, 6), 0.99, toolSet.RandomRange(120, 180), Color(255, 163, 0));
			particleController.AddParticle(toolSet.RandomRange(-500, 800), 300, 270, toolSet.RandomRange(3, 6), 0.97, toolSet.RandomRange(3, 6), 0.99, toolSet.RandomRange(120, 180), Color(95, 87, 79));
			particleController.AddParticle(toolSet.RandomRange(-500, 800), 300, 270, toolSet.RandomRange(3, 6), 0.97, toolSet.RandomRange(3, 6), 0.99, toolSet.RandomRange(120, 180), Color(255, 0, 77));
			particleController.AddParticle(toolSet.RandomRange(-500, 800), 300, 270, toolSet.RandomRange(6, 12), 0.97, toolSet.RandomRange(3, 6), 0.99, toolSet.RandomRange(120, 180), Color(255, 241, 232));

		}

		//Move the good ship upwards
		evilBaseRotation += 0.2; //Rotate the base sprite

		goodShipPos.y -= 3;

		goodShip.setPosition(goodShipPos);
		evilBase.setPosition(evilBasePos);
		evilBase.setRotation(evilBaseRotation);

		gameRenderer->AddDrawable(25, evilBase);
		gameRenderer->AddDrawable(50, goodShip);

		//Update particles
		particleController.Update();

		//Count down until next scene
		sceneFrames -= 1;

		if (sceneFrames <= 0)
		{

			sceneFrames = 1000;
			endingState = ENDING_SPACEFLYBY;

			//Reset sprite positions for next frame
			goodShipPos = Vector2f(-600, 20);
			goodShip.setScale(7, 7);

			//Add initial text
			textController->AddNewTextBox("...So now what? Do we go home?");
			textController->AddNewTextBox("It seems like it! Now that we're finally safe. We're all heroes!");

			//Clear particles
			particleController.ClearAllParticles();
		}

		break;

	case ENDING_SPACEFLYBY:

		//Particles
		particleController.Update();

		//Move the good ship to the right
		goodShipPos.x += 2;
		goodShip.setPosition(goodShipPos);

		gameRenderer->AddDrawable(50, goodShip);

		for (int i = 0; i < 1; i++)
		{

			//Stars
			particleController.AddParticle(700, toolSet.RandomRange(-300, 400), 180, toolSet.RandomRange(3, 7), 1, toolSet.RandomRange(1, 2), 1, 180, Color::White);

			//Ship thrust
			particleController.AddParticle(goodShipPos.x - 13, goodShipPos.y + 80, 180, 0.01, 1, toolSet.RandomRange(5, 12), 0.99, toolSet.RandomRange(60, 120), Color(255, 163, 0));
			particleController.AddParticle(goodShipPos.x - 13, goodShipPos.y + 80, 180, 0.01, 1, toolSet.RandomRange(5, 12), 0.99, toolSet.RandomRange(60, 120), Color(95, 87, 79));

		}

		//Count down until next scene
		sceneFrames -= 1;

		if (sceneFrames <= 0)
		{

			sceneFrames = 2400;
			endingState = ENDING_REVEAL;

			//Reset sprite positions for next frame
			goodShipPos = Vector2f(-300, 0);
			goodShip.setScale(3, 3);

			//Add initial text
			textController->AddNewTextBox("...Captain? What's that on the radar?");
			textController->AddNewTextBox("I'm sure it's nothing.");
			textController->AddNewTextBox("Really, because it looks like the secret weapon.");
			textController->AddNewTextBox("Definitely not.");
			textController->AddNewTextBox("Hmmm... Weird alien tech, giant guns, nope. It's absolutely the secret weapon.");
			textController->AddNewTextBox("...");
			textController->AddNewTextBox("...");
			textController->AddNewTextBox("...I quit.");
			textController->AddNewTextBox("THE END NOW PLAY MY SEMESTER 1 ASSIGNMENT :)");

		}

		break;

	case ENDING_REVEAL:

		//Particles
		particleController.Update();

		//Move the good ship to the right
		evilShipPos.x += 2;
		goodShipPos.x += 0.3;
		evilShip.setPosition(evilShipPos);
		goodShip.setPosition(goodShipPos);

		gameRenderer->AddDrawable(50, evilShip);
		gameRenderer->AddDrawable(30, goodShip);

		for (int i = 0; i < 1; i++)
		{

			//Stars
			particleController.AddParticle(700, toolSet.RandomRange(-300, 400), 180, toolSet.RandomRange(3, 7), 1, toolSet.RandomRange(1, 2), 1, 180, Color::White);

			//Evil Ship thrust
			particleController.AddParticle(evilShipPos.x - 60, evilShipPos.y + 80, 180, 0.01, 1, toolSet.RandomRange(5, 12), 0.99, toolSet.RandomRange(60, 120), Color(255, 163, 0));
			particleController.AddParticle(evilShipPos.x - 60, evilShipPos.y + 80, 180, 0.01, 1, toolSet.RandomRange(5, 12), 0.99, toolSet.RandomRange(60, 120), Color(95, 87, 79));

			//Good Ship thrust
			particleController.AddParticle(goodShipPos.x - 13, goodShipPos.y + 40, 180, 0.01, 1, toolSet.RandomRange(1, 3), 0.99, toolSet.RandomRange(60, 120), Color(255, 163, 0));
			particleController.AddParticle(goodShipPos.x - 13, goodShipPos.y + 40, 180, 0.01, 1, toolSet.RandomRange(1, 3), 0.99, toolSet.RandomRange(60, 120), Color(95, 87, 79));

		}

		//Count down until next scene
		sceneFrames -= 1;

		if (sceneFrames <= 0)
		{

			endingReset = true;

		}

		break;

	}

}

void EndingController::Reset()
{

	//Reset the ending controller
	goodShipPos = Vector2f(120, 135);
	evilShipPos = Vector2f(-600, 50);
	evilBasePos = Vector2f(120, 135);

	endingReset = false;
	evilBaseRotation = 0;
	sceneFrames = 500;

}