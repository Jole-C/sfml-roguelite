#include "PlayerController.h"
#include "EnemyController.h"

using namespace sf;

void Player::Initialise(PlayerController* _playerController, GameRenderer* _renderer, RoomController* _roomController, SpriteLoader* _spriteLoader, BulletController* _bulletController, ParticleController* _particleController, int _x, int _y)
{

	//set up the pointers used by the player
	gameRenderer = _renderer;
	roomController = _roomController;
	playerController = _playerController;
	particleController = _particleController;
	spriteLoader = _spriteLoader;
	bulletController = _bulletController;

	//set position
	x = _x;
	y = _y;

	//set the player sprites
	sprites = spriteLoader->GetTexture("spritesheet.png");
	
	overlayTexture = spriteLoader->GetTexture("overlay.png");
	overlaySprite.setTexture(overlayTexture);
	overlaySprite.setOrigin(overlaySprite.getGlobalBounds().width / 2, overlaySprite.getGlobalBounds().height / 2);

	IntRect rect;

	rect.top = 144;
	rect.left = 0;
	rect.width = 16;
	rect.height = 16;
	playerBase.setTextureRect(rect);
	playerBase.setTexture(sprites);

	rect.top = 128;
	rect.left = 0;
	rect.width = 16;
	rect.height = 16;
	playerCannon.setTextureRect(rect);
	playerCannon.setTexture(sprites);

	playerBase.setPosition(x, y);
	playerCannon.setPosition(x, y);

	playerBase.setScale(3, 3);
	playerCannon.setScale(3, 3);

	playerBase.setOrigin(8, 8);
	playerCannon.setOrigin(8, 8);

	//Set the melee sprite
	rect.top = 82;
	rect.left = 1;
	rect.width = 10;
	rect.height = 14;

	meleeWeapon.setTexture(sprites);
	meleeWeapon.setTextureRect(rect);
	meleeWeapon.setScale(7, 7);
	meleeWeapon.setOrigin(5, 7);

}

void Player::Reset()
{

	bulletCount = 5;
	maxBulletCount = 5;

	//Firing variables
	fireCooldown = 45;
	maxFireCooldown = 45;
	bulletRechargeGracePeriod = 70;
	maxBulletRechargeGracePeriod = 70;
	bulletRechargeRate = 30;
	maxBulletRechargeRate = 30;

	//Health
	health = 5;
	maxHealth = 5;
	deathState = 0;
	invunTime = 60;
	spawnCooldown = 60;

	//Movement
	baseSpeed = 3;

	x = 512;
	y = 512;

}

void Player::Update()
{

	//Upon spawn
	spawnCooldown -= 1;

	//Move the player
	switch (deathState)
	{

		case 0:
		{

			if (y > roomController->MAX_ROOM_POS_Y)y = roomController->MAX_ROOM_POS_Y;
			if (y < roomController->MIN_ROOM_POS_Y)y = roomController->MIN_ROOM_POS_Y;
			if (x > roomController->MAX_ROOM_POS_X)x = roomController->MAX_ROOM_POS_X;
			if (x < roomController->MIN_ROOM_POS_X)x = roomController->MIN_ROOM_POS_X;

			if (Keyboard::isKeyPressed(Keyboard::W) && (roomController->GetTile(x, y - 1) == 45 || roomController->GetTile(x, y - 1) == 5))
			{
				ySpeed = -baseSpeed;
			}

			if (Keyboard::isKeyPressed(Keyboard::S) && (roomController->GetTile(x, y + 1) == 45 || roomController->GetTile(x, y + 1) == 5))
			{
				ySpeed = baseSpeed;
			}

			if (Keyboard::isKeyPressed(Keyboard::A) && (roomController->GetTile(x - 1, y) == 45 || roomController->GetTile(x - 1, y) == 5))
			{
				xSpeed = -baseSpeed;
			}

			if (Keyboard::isKeyPressed(Keyboard::D) && (roomController->GetTile(x + 1, y) == 45 || roomController->GetTile(x + 1, y) == 5))
			{
				xSpeed = baseSpeed;
			}

			//increment animation frame
			if (xSpeed != 0 || ySpeed != 0)
			{
				animationFrame += 0.2;
			}

			//set the angle of the player base sprite to a variable
			double angle = atan2(ySpeed, xSpeed);

			//set the angle of the cannon rotation to a variable
			Vector2f mousePos = gameRenderer->window.mapPixelToCoords(Mouse::getPosition(gameRenderer->window));
			double aimAngle = atan2(mousePos.y - y, mousePos.x - x);

			//apply these angles to the appropriate sprite
			playerBase.setRotation(toolSet.RadiansToDegrees(angle) + 90);
			playerCannon.setRotation(toolSet.RadiansToDegrees(aimAngle) + 90);

			//update player position based on speed
			x += xSpeed;
			y += ySpeed;

			//reset the speed
			xSpeed = 0;
			ySpeed = 0;

			//set the sprite positions
			playerBase.setPosition(x, y);
			playerCannon.setPosition(x, y);

			//push the sprites to the renderer queue after all modifications have been applied
			gameRenderer->AddDrawable(DEPTH_GAME_MIN, playerBase);
			gameRenderer->AddDrawable(DEPTH_GAME_MIN + 1, playerCannon);

			//make the camera follow the player
			if (roomController->spawnBoss == false)
			{
				gameRenderer->ViewSetFollowCoordinates(x, y);
			}

			//handle exiting rooms
			Vector2i positionModifier = roomController->RoomTransition(x, y);

			//if the position modifier for the given door is not equal to 0, use this to set their position in the next room.
			//EG: door left has a position modifier of -1. This ensures that when the player leaves that room, the roomcontroller knows to subtract 1 from the room coordinates, thereby loading the left room.
			//By using this and reversing the negative value, we can add 512 to the player's position after setting them to the center of the next room, thereby making it seem like they come out of the right side door.
			if (positionModifier.x != 0 || positionModifier.y != 0)
			{

				x = 512 + (512 * -positionModifier.x);
				y = 512 + (512 * -positionModifier.y);

				particleController->ClearAllParticles();

			}

			//handle bullet firing
			fireCooldown -= 1;


			switch (playerWeapon)
			{

				case 0:
				{

					if (fireCooldown <= 0 && Mouse::isButtonPressed(Mouse::Button::Left) && bulletCount > 0)
					{

						gameRenderer->ScreenShake(300, 3);

						bulletCount -= 1;

						bulletController->AddNewBullet(x, y, aimAngle, 12, 60);
						fireCooldown = maxFireCooldown;
						bulletRechargeGracePeriod = maxBulletRechargeGracePeriod;

						//particles
						for (int i = 0; i < 12; i++)
						{
							particleController->AddParticle(x, y, toolSet.RadiansToDegrees(aimAngle) + toolSet.RandomRange(-15, 15), toolSet.RandomRange(5, 10), 0.995, toolSet.RandomRange(3, 6), 0.995, toolSet.RandomRange(10, 30), Color(30, 85, 181));
						}

					}

					//recharge the bullets
					bulletRechargeGracePeriod -= 1;
					if (bulletRechargeGracePeriod <= 0)
					{

						bulletRechargeRate -= 1;

						if (bulletRechargeRate <= 0)
						{

							if (bulletCount < maxBulletCount)
							{
								bulletCount += 1;
							}
							else
							{
								bulletRechargeGracePeriod = 120;
							}

							bulletRechargeRate = maxBulletRechargeRate;

						}

					}

					break;

				}

				case 1:
				{

					if (Mouse::isButtonPressed(Mouse::Left) && meleeCooldown <= -120)
					{

						meleeCooldown = maxMeleeCooldown;

						//Particles
						for (int i = 0; i < 7; i++)
						{
							double particleAngle = toolSet.DegreesToRadians(toolSet.RadiansToDegrees(aimAngle) + toolSet.RandomRange(-30, 30));
							particleController->AddParticle(x, y, particleAngle, toolSet.RandomRange(6, 12), 0.95, toolSet.RandomRange(1, 5), 0.99, 180, Color(255, 241, 232));
						}

					}

					//Draw the melee sprite
					if (meleeCooldown > 0)
					{

						meleeWeapon.setRotation(toolSet.RadiansToDegrees(aimAngle) + 90);
						meleeWeapon.setPosition(x, y);

						gameRenderer->AddDrawable(DEPTH_GAME_MAX, meleeWeapon);

					}

					break;

				}

			}

			//lower melee cooldown regardless of weapon type
			//to prevent constantly oneshotting enemies
			meleeCooldown -= 1;

			//lower invulnerability
			invunTime -= 1;

			//set the texture rect for frames
			IntRect rect;

			rect.top = 144;
			rect.left = 0 + (floor(animationFrame) * 16);
			rect.width = 16;
			rect.height = 16;

			if (animationFrame > 2.8)animationFrame = 0;

			playerBase.setTextureRect(rect);

			//Switch weapon
			if (Keyboard::isKeyPressed(Keyboard::Num1))
			{
				playerWeapon = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num2))
			{
				playerWeapon = 1;
			}

			//Die
			if (health <= 0)
			{

				deathState = 1;

				//Death particles
				for (int i = 0; i < 15; i++)
				{

					particleController->AddParticle(x, y, toolSet.RandomRange(0, 360), toolSet.RandomRange(4, 15), 0.95, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(255, 0, 77));
					particleController->AddParticle(x, y, toolSet.RandomRange(0, 360), toolSet.RandomRange(5, 7), 0.95, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(194, 195, 199));
					particleController->AddParticle(x, y, toolSet.RandomRange(0, 360), toolSet.RandomRange(3, 5), 0.9, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(95, 87, 79));
					particleController->AddParticle(x, y, toolSet.RandomRange(0, 360), toolSet.RandomRange(5, 12), 0.9, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(255, 163, 0));

				}

			}

			break;

		}

		//Death state
		case 1:
		{


			break;
		}

	}

	//Overlay
	overlaySprite.setPosition(x, y);
	gameRenderer->AddDrawable(DEPTH_GUI_MAX, overlaySprite);

}

void PlayerController::Initialise(GameRenderer* _renderer, RoomController* _roomController, SpriteLoader* _spriteLoader, int _x, int _y)
{

	//set up the gamerenderer and roomcontroller pointers
	gameRenderer = _renderer;
	roomController = _roomController;
	spriteLoader = _spriteLoader;

	//set up the bullet controller
	bulletController.Initialise(15, &player, spriteLoader, gameRenderer, roomController);
	bulletController.InitialiseBullets(Vector2f(0, 36), Vector2f(8, 10), true);

	//set up the particle controller
	particleController.Initialise(150, gameRenderer, spriteLoader);

	//set up the player
	player.Initialise(this, gameRenderer, roomController, spriteLoader, &bulletController, &particleController, _x, _y);
	
	//Set up the sprites
	spritesheetTexture = spriteLoader->GetTexture("spritesheet.png");

	healthOutline.setTexture(spritesheetTexture);
	healthOutline.setTextureRect(IntRect(240, 97, 80, 30));
	healthOutline.setScale(3, 3);

	healthInner.setTexture(spritesheetTexture);
	healthInner.setTextureRect(IntRect(240, 65, 80, 30));
	healthInner.setScale(3, 3);

	bulletEmpty.setTexture(spritesheetTexture);
	bulletEmpty.setTextureRect(IntRect(304, 128, 16, 32));
	bulletEmpty.setScale(3, 3);

	bulletFull.setTexture(spritesheetTexture);
	bulletFull.setTextureRect(IntRect(288, 128, 16, 32));
	bulletFull.setScale(3, 3);

	playerWeapon.setTexture(spritesheetTexture);
	playerWeapon.setTextureRect(IntRect(0, 70, 11, 11));
	playerWeapon.setScale(7, 7);

}

void PlayerController::Update()
{

	//Do player collisions
	for (int i = 0; i < enemyController->bulletController.bulletPool.size(); i++)
	{

		//If the bullet is alive
		if (enemyController->bulletController.bulletPool[i].IsAvailable() == false)
		{

			//Do distance check
			if (toolSet.Distance(enemyController->bulletController.bulletPool[i].x, enemyController->bulletController.bulletPool[i].y, player.x, player.y) < 70 && player.invunTime <= 0)
			{

				player.health -= 1;
				player.invunTime = 60;
				enemyController->bulletController.bulletPool[i].life = -1;

				//Fire particles
				particleController.AddParticle(player.x, player.y, 0, 0, 0, 40, 0.8, toolSet.RandomRange(120, 180), Color::White);

				for (int i = 0; i < 15; i++)
				{
					particleController.AddParticle(player.x, player.y, toolSet.RandomRange(0, 360), toolSet.RandomRange(5, 12), 0.9, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(255, 163, 0));
				}

			}

		}

	}

	//update the player, bullet and particle controller
	bulletController.Update();
	player.Update();
	particleController.Update();

	//Get view coordinates
	int viewX = gameRenderer->GetViewPosition().x;
	int viewY = gameRenderer->GetViewPosition().y;
	int viewSizeX = gameRenderer->GetViewSize().x;
	int viewSizeY = gameRenderer->GetViewSize().y;

	//draw the health bar
	int hpX = viewX + viewSizeX / 2 - healthOutline.getGlobalBounds().width - 2;
	int hpY = viewY - viewSizeY / 2 + 2;

	//Healthbar texture rect
	IntRect healthRect(240, 65, floor((player.health / player.maxHealth) * 72), 30);
	healthInner.setTextureRect(healthRect);

	healthOutline.setPosition(hpX, hpY);
	healthInner.setPosition(hpX, hpY);

	gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX + 1, healthOutline);
	gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX, healthInner);

	//draw the empty bullet shells
	//Get the base position for bullet icons
	int spriteSizeX = bulletEmpty.getGlobalBounds().width; //Get sprite width
	int spriteGap = 10; //The gap in pixels between each sprite

	int basePosX = (viewX + viewSizeX / 2) - ((spriteSizeX * player.maxBulletCount) + (spriteGap * player.maxBulletCount));
	int basePosY = (gameRenderer->GetViewPosition().y + gameRenderer->GetViewSize().y / 2) - bulletEmpty.getGlobalBounds().height;
	
	for (int i = 0; i < player.maxBulletCount; i++)
	{

		//Draw the empty bullet
		bulletEmpty.setPosition(basePosX + i * (spriteSizeX + spriteGap), basePosY);
		gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX, bulletEmpty);

	}

	//draw the bullet shells using the above math but for the number of actual bullets the player has
	for (int i = 0; i < player.bulletCount; i++)
	{

		//Draw the empty bullet
		bulletFull.setPosition(basePosX + i * (spriteSizeX + spriteGap), basePosY);
		gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX, bulletFull);

	}

	//draw the sprite for the player weapon
	int playerWeaponID = player.playerWeapon;

	IntRect playerWeaponRect(0 + (11 * playerWeaponID), 70, 11, 11);

	playerWeapon.setTextureRect(playerWeaponRect);
	playerWeapon.setPosition(viewX - viewSizeX / 2 + 1, viewY + viewSizeY / 2 - 77);

	gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX, playerWeapon);

	//Player death
	if (player.deathState == 1)
	{
		deathCooldown -= 1;
	}

}


void PlayerController::Reset()
{
	player.Reset();
	particleController.ClearAllParticles();
	bulletController.Destroy();
	deathCooldown = 180;
}