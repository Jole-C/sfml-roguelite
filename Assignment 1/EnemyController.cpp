// EnemyController.cpp : This class controls all enemies.

#include "EnemyController.h"
#include "PlayerController.h"

void EnemyController::Initialise(GameRenderer* _renderer, SpriteLoader* _spriteLoader, PlayerController* _playerController, RoomController* _roomController, TextController* _textController, BackgroundController* _backgroundController)
{

	//Initialise pointers
	gameRenderer = _renderer;
	spriteLoader = _spriteLoader;
	playerController = _playerController;
	roomController = _roomController;
	textController = _textController;
	backgroundController = _backgroundController;

	//Initialise bullet controller
	bulletController.Initialise(250, &playerController->player, spriteLoader, gameRenderer, roomController);
	bulletController.InitialiseBullets(Vector2f(8, 34), Vector2f(14, 14), false);

	//Initialise particles
	particleController.Initialise(150, gameRenderer, spriteLoader);

	//Load alarm
	if (!alarm.openFromFile("alarm.wav"))
	{
		toolSet.ConsoleOut("Couldn't load sound.");
	}

}

void EnemyController::AddEnemy(int _x, int _y, int _patrolRadius, ENEMY_TYPE _enemyType)
{

	switch (_enemyType)
	{

		case ENEMY_DEFAULT:
		{
			EnemyDefault* newEnemy = new EnemyDefault;
			newEnemy->Initialise(_x, _y, gameRenderer, playerController, spriteLoader, roomController, &bulletController, &particleController);
			newEnemy->patrolRadius = _patrolRadius;
			enemyPool.push_back(newEnemy);
			toolSet.ConsoleOut("Added default enemy.");
			break;
		}

		case ENEMY_SNIPER:
		{
			EnemySniper* newEnemy = new EnemySniper;
			newEnemy->Initialise(_x, _y, gameRenderer, playerController, spriteLoader, roomController, &bulletController, &particleController);
			newEnemy->patrolRadius = _patrolRadius;
			enemyPool.push_back(newEnemy);
			toolSet.ConsoleOut("Added sniper enemy.");
			break;
		}

		case ENEMY_SPREADER:
		{
			EnemySpreader* newEnemy = new EnemySpreader;
			newEnemy->Initialise(_x, _y, gameRenderer, playerController, spriteLoader, roomController, &bulletController, &particleController);
			newEnemy->patrolRadius = _patrolRadius;
			enemyPool.push_back(newEnemy);
			toolSet.ConsoleOut("Added spreader enemy.");
			break;
		}

		case BOSS_SHIELD:
		{
			EnemyShieldBoss* newEnemy = new EnemyShieldBoss;
			newEnemy->Initialise(_x, _y, gameRenderer, playerController, spriteLoader, roomController, &bulletController, &particleController, backgroundController);
			newEnemy->patrolRadius = _patrolRadius;
			enemyPool.push_back(newEnemy);
			toolSet.ConsoleOut("Added shield boss enemy.");
			break;
		}

		case BOSS_MAINFRAMEGUARD:
		{
			EnemyGuardBoss* newEnemy = new EnemyGuardBoss;
			newEnemy->Initialise(_x, _y, gameRenderer, playerController, spriteLoader, roomController, &bulletController, &particleController, backgroundController);
			newEnemy->patrolRadius = _patrolRadius;
			newEnemy->enemyState = ENEMY_STATES::ALERT_COMBAT;
			enemyPool.push_back(newEnemy);
			toolSet.ConsoleOut("Added guard boss enemy.");
			break;
		}
		default:
		{
			EnemyDefault* newEnemy = new EnemyDefault;
			newEnemy->Initialise(_x, _y, gameRenderer, playerController, spriteLoader, roomController, &bulletController, &particleController);
			newEnemy->patrolRadius = _patrolRadius;
			enemyPool.push_back(newEnemy);
			toolSet.ConsoleOut("Added default enemy.");
			break;
		}

	}

}

void EnemyController::Update()
{

	//Update particles
	particleController.Update();

	//Iterate over enemies, removing dead ones
	auto iterator = enemyPool.begin();

	while (iterator != enemyPool.end())
	{

		if ((*iterator)->life <= 0)
		{

			//Death particles
			for (int i = 0; i < 15; i++)
			{

				particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(4, 15), 0.95, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(255, 0, 77));
				particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(5, 7), 0.95, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(194, 195, 199));
				particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(3, 5), 0.9, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(95, 87, 79));
				particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(5, 12), 0.9, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(255, 163, 0));

			}

			//If the enemy is a boss, start the ending sequence
			if ((*iterator)->isBoss == true && playerController->player.health > 0 && playerController->player.spawnCooldown <= 0)
			{

				//Add escape text
				textController->AddNewTextBox("WARNING! WARNING! YOU MESSED UP - FACILITY DETONATION IN 100 SECONDS");
				textController->AddNewTextBox("I have to get out of here! The start room should be the exit...");

				//Death particles
				for (int i = 0; i < 75; i++)
				{

					particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(20, 30), 1, toolSet.RandomRange(2, 7), 1, toolSet.RandomRange(120, 180), Color(255, 0, 77));
					particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(20, 30), 1, toolSet.RandomRange(2, 7), 1, toolSet.RandomRange(120, 180), Color(194, 195, 199));
					particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(20, 30), 1, toolSet.RandomRange(2, 7), 1, toolSet.RandomRange(120, 180), Color(95, 87, 79));
					particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(20, 30), 1, toolSet.RandomRange(2, 7), 1, toolSet.RandomRange(120, 180), Color(255, 163, 0));

				}

				alarm.stop();

			}

			//If the enemy is a shield boss, upgrade the player
			if ((*iterator)->isMiniBoss == true && playerController->player.health > 0 && playerController->player.spawnCooldown <= 0)
			{

				//Stop music
				(*iterator)->miniBossMusic.stop();

				//Change the background colour
				backgroundController->SetColour(Color(255, 255, 255));

				//Lower number of trap rooms
				roomController->trapRoomsCompleted += 1;

				for (int i = 0; i < 75; i++)
				{
					particleController.AddParticle((*iterator)->x, (*iterator)->y, toolSet.RandomRange(0, 360), toolSet.RandomRange(15, 25), 0.95, toolSet.RandomRange(2, 60), 0.95, toolSet.RandomRange(120, 180), Color::White);
				}

				int chosenText = toolSet.RandomRange(0, 5);

				switch (chosenText)
				{
					case 0:
						textController->AddNewTextBox("Looks like the residual energy from that shield generator upgraded your tank!");
						textController->AddNewTextBox("...Is that dangerous?");
						textController->AddNewTextBox("Probably only a small chance of exploding.");
						textController->AddNewTextBox("NEW UPGRADE:\nBULLETS NOW RECHARGE QUICKER\nBULLETS GO BRRRRRR");
						playerController->player.maxBulletRechargeGracePeriod -= 15;
						playerController->player.maxBulletRechargeRate -= 5;
						playerController->player.maxFireCooldown -= 5;
					break;
					case 1:
						textController->AddNewTextBox("OW! THAT HURT!");
						textController->AddNewTextBox("Oops. Looks like R&D forgot to install the energy shields. Oh well, at least your tank was upgraded.");
						textController->AddNewTextBox("Freaking jerks.");
						textController->AddNewTextBox("NEW UPGRADE:\nMORE HEALTH, IRONICALLY");
						playerController->player.maxHealth += 1;
						if (playerController->player.health < playerController->player.maxHealth)
						{
							playerController->player.health += 1;
						}
					break;
					case 2:
						textController->AddNewTextBox("Oooo look at those pretty colours!");
						textController->AddNewTextBox("Probably shouldn't stare at them for too long, you might go blind. Theoretically though, they should upgrade your tank.");
						textController->AddNewTextBox("NEW UPGRADE:\nWHO NEEDS EYES WHEN YOU HAVE MORE BULLETS?");
						playerController->player.maxBulletCount += 2;
					break;
					case 3:
						textController->AddNewTextBox("Oooo look at those pretty colours!");
						textController->AddNewTextBox("Probably shouldn't stare at them for too long, you might go blind. Theoretically though, they should upgrade your tank.");
						textController->AddNewTextBox("NEW UPGRADE:\nWHO NEEDS EYES WHEN YOU HAVE MORE BULLETS?");
						playerController->player.maxBulletCount += 2;
						break;
					case 4:
						textController->AddNewTextBox("Uh, was that meant to do something?");
						textController->AddNewTextBox("Probably.");
						textController->AddNewTextBox("NEW UPGRADE:\nYOU GET NOTHING, YOU LOSE. GOOD DAY SIR.");
					break;
				}

			}

			//Delete the enemy and fix the invalid iterator, decreasing the room's number of enemies
			delete(*iterator);

			roomController->numberOfEnemies -= 1;

			iterator = enemyPool.erase(iterator);

		}
		else
		{

			//If enemy is boss and hp is low enough, play the alarm sound

			if ((*iterator)->isBoss == true)
			{
				if (playAlarm == true && (*iterator)->life <= 15)
				{
					playAlarm = false;
					alarm.setLoop(true);
					alarm.play();
					endingSequence = true;
				}
			}


			//Update the enemy
			(*iterator)->Update();

			//Make nearby enemies alert if the current enemy enters combat
			if ((*iterator)->enemyState == ENEMY_STATES::ALERT_COMBAT)
			{

				for (int i = 0; i < enemyPool.size(); i++)
				{

					if (toolSet.Distance((*iterator)->x, (*iterator)->y, enemyPool[i]->x, enemyPool[i]->y) < enemyAlertRadius && enemyPool[i]->enemyState != ENEMY_STATES::ALERT_COMBAT && enemyPool[i]->enemyState != ENEMY_STATES::ALERT_SEARCHING)
					{
						enemyPool[i]->enemyState = ENEMY_STATES::ALERT_SEARCHING;
					}

				}

			}

			//Increase the iterator
			iterator += 1;

		}
	}

	//Update bullets
	bulletController.Update();

}



//ENEMY CODE
void EnemyBase::GenerateViewConeSprite()
{

	//Generate a vertex array
	VertexArray viewCone(Triangles, 3);

	viewCone[0].position = Vector2f(0, viewConeRadius);
	viewCone[1].position = Vector2f(viewConeLength, 0);
	viewCone[2].position = Vector2f(viewConeLength, viewConeRadius * 2);

	viewCone[0].color = Color::Color(255, 255, 255, 255);
	viewCone[1].color = Color::Color(255, 255, 255, 0);
	viewCone[2].color = Color::Color(255, 255, 255, 0);

	//Draw it to a render texture
	viewConeTexture.create(viewConeLength, viewConeRadius * 2);
	viewConeTexture.draw(viewCone);

	//Set the sprite's texture
	viewConeSprite.setTexture(viewConeTexture.getTexture());
	viewConeSprite.setColor(Color(255, 255, 255, 50));
	viewConeSprite.setOrigin(0, viewConeSprite.getGlobalBounds().height / 2);

}

void EnemyBase::Patrol()

{

	switch (patrolState)
	{
		case PATROL_STATES::SET_PATROL_MOVE_GOAL:
		{

			//Generate a random position within the patrol radius circle

			//Pick angle
			double angle = toolSet.DegreesToRadians(toolSet.RandomRange(0, 360));

			int targetx = patrolBasePosition.x + cos(angle) * patrolRadius;
			int targety = patrolBasePosition.y + sin(angle) * patrolRadius;

			//Set them back into the vector
			targetPatrolPosition = Vector2f(targetx, targety);

			//Switch state
			patrolState = PATROL_STATES::PATROL_MOVE;

			break;

		}
		case PATROL_STATES::PATROL_MOVE:
		{

			//Move to the target position, stop once reached, or if the cooldown reaches 0
			patrolMoveCooldown -= 1;

			if (patrolMoveCooldown > 0)
			{

				//If target position reached, stop moving
				if (toolSet.Distance(x, y, targetPatrolPosition.x, targetPatrolPosition.y) < 3)
				{
					patrolMoveCooldown = -1;
				}

				//Get the angle between the enemy and the patrol position
				double angle = toolSet.AngleBetweenPoints(x, y, targetPatrolPosition.x, targetPatrolPosition.y);

				//Rotate the sprites and move the enemy to the patrol position
				baseAngle = toolSet.RadiansToDegrees(angle + 90);
				patrolCannonAngle = toolSet.RadiansToDegrees(angle + 90);

				tankAngle = toolSet.bisectAngle(tankAngle, patrolCannonAngle, 0.025);

				MoveEnemy(angle);

				base.setRotation(toolSet.RadiansToDegrees(angle + 90));
				tank.setRotation(tankAngle);

			}
			//If the patrol cooldown is less than 0, switch state
			else
			{

				//Pick a random patrol cannon angle for in advance of the next state
				patrolCannonAngle = toolSet.constrainAngle(toolSet.RandomRange(tankAngle - 100, tankAngle + 100));

				//Switch to the next state
				patrolState = PATROL_STATES::LOOK_AROUND;

				//Reset cooldown
				patrolMoveCooldown = maxPatrolMoveCooldown;

			}

			break;

		}
		case PATROL_STATES::LOOK_AROUND:
		{

			//Count down for state change
			patrolMoveCooldown -= 1;

			//Rotate to the previously set angle
			tankAngle = toolSet.bisectAngle(tankAngle, patrolCannonAngle, 0.025);

			tank.setRotation(tankAngle);

			//Switch back to the first state
			if (patrolMoveCooldown <= 0)
			{
				patrolState = PATROL_STATES::SET_PATROL_MOVE_GOAL;
				patrolMoveCooldown = maxPatrolMoveCooldown;
			}

			break;

		}

	}

}

void EnemyBase::Update()
{

	//Execute enemy behaviour
	Player player = playerController->player;
	angleToPlayer = toolSet.AngleBetweenPoints(x, y, player.x, player.y);

	switch (enemyState)
	{

		case ENEMY_STATES::PATROL:
		{

			//Update sprite colour
			viewConeSprite.setColor(Color::White);

			if (isMiniBoss == false && isBoss == false)
			{
				//Patrol
				Patrol();
			}
			else if (isMiniBoss == true)
			{
				tankAngle += 0.5;
			}

			//If player is within view cone, switch to alert
			double angleToPlayer = toolSet.RadiansToDegrees(toolSet.AngleBetweenPoints(playerController->player.x, playerController->player.y, x, y)) - 90;

			//Check if the angle to the player is within the confines of the view cone
			double distanceToPlayer = toolSet.Distance(x, y, playerController->player.x, playerController->player.y);

			patrolGracePeriod -= 1;

			if (patrolGracePeriod <= 0)
			{

				if( (toolSet.isBetween(tankAngle - viewConeRadius / 4, tankAngle + viewConeRadius / 4, angleToPlayer) == true || toolSet.Distance(playerController->player.x, playerController->player.y, x, y) < hearingRadius && Mouse::isButtonPressed(Mouse::Left)) && distanceToPlayer < viewConeLength)
				{
					enemyState = ENEMY_STATES::ALERT_SEARCHING;
					patrolGracePeriod = 180;
					minimumAlertPeriod = 120;
				}

			}

			break;

		}

		case ENEMY_STATES::ALERT_SEARCHING:
		{

			//Update sprite colour
			viewConeSprite.setColor(Color::Yellow);

			Alert();
			break;

		}

		case ENEMY_STATES::ALERT_COMBAT:
		{

			//Update sprite colour
			viewConeSprite.setColor(Color::Red);

			Chase();
			break;

		}
		case ENEMY_STATES::DEATH:
		{

			Death();
			break;

		}

	}

	//Render the sprites
	base.setPosition(x, y);
	tank.setPosition(x, y);
	gameRenderer->AddDrawable(DEPTH_GAME_MIN + 2, base);
	gameRenderer->AddDrawable(DEPTH_GAME_MIN + 4, tank);

	if (isBoss == false)
	{

		viewConeSprite.setPosition(x, y);
		viewConeSprite.setRotation(tankAngle - 90);
		gameRenderer->AddDrawable(DEPTH_GAME_MIN + 3, viewConeSprite);

	}

	//Melee collision
	if (playerController->player.meleeCooldown > 0 && toolSet.Distance(x, y, playerController->player.x, playerController->player.y) < 120 && enemyState == ENEMY_STATES::PATROL)
	{
		life = -1;
	}


	//Bullet collision
	invunTime -= 1;
	for (int i = 0; i < playerController->bulletController.bulletPool.size(); i++)
	{

		if (invunTime <= 0 && playerController->bulletController.bulletPool[i].IsAvailable() == false)
		{

			int bulletX = playerController->bulletController.bulletPool[i].x;
			int bulletY = playerController->bulletController.bulletPool[i].y;

			if (toolSet.Distance(x, y, bulletX, bulletY) < collisionDistance)
			{

				//Make the enemy enter combat
				enemyState = ENEMY_STATES::ALERT_COMBAT;

				//Decrease life
				playerController->bulletController.bulletPool[i].life = -1;
				life -= 1;
				invunTime = 15;

				//Fire particles
				particleController->AddParticle(x, y, 0, 0, 0, 40, 0.8, toolSet.RandomRange(120, 180), Color::White);

				for (int i = 0; i < 15; i++)
				{
					particleController->AddParticle(x, y, toolSet.RandomRange(0, 360), toolSet.RandomRange(5, 12), 0.9, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(255, 163, 0));
				}

				//Play miniboss music
				if (isMiniBoss == true)
				{

					if (playMusic == true)
					{
						playMusic = false;
						miniBossMusic.play();
					}

				}

			}

		}

	}

}

void EnemyBase::MoveEnemy( double _angle)
{

	//X movement/collision
	int xCheckDir = cos(_angle) * patrolSpeed;
	int yCheckDir = sin(_angle) * patrolSpeed;

	if (roomController->GetTile(x + xCheckDir, y) == 5 || roomController->GetTile(x + xCheckDir, y) == 45)
	{
		x += cos(_angle) * patrolSpeed;
	}

	if (roomController->GetTile(x, y + yCheckDir) == 5 || roomController->GetTile(x, y + yCheckDir) == 45)
	{
		y += sin(_angle) * patrolSpeed;
	}

	//Animate
	tankFrame += 0.2;

	tankAnimRect.left = tankRectModifier * floor(tankFrame);

	if (tankFrame >= 2.8)
	{
		tankFrame = 0;
	}

	base.setTextureRect(tankAnimRect);

}

void EnemyBase::Alert()
{

	double angleToPlayer = toolSet.RadiansToDegrees(toolSet.AngleBetweenPoints(x, y, playerController->player.x, playerController->player.y)) + 90;
	tankAngle = toolSet.bisectAngle(tankAngle, angleToPlayer, 0.025);

	tank.setRotation(tankAngle);

	//If the minimum alert period is less than 0,
	minimumAlertPeriod -= 1;

	if (minimumAlertPeriod <= 0 && permanentCombat == false)
	{

		alertGracePeriod -= 1;

		//If the player is not within the view cone go back to normal
		if (toolSet.Distance(x, y, playerController->player.x, playerController->player.y) > viewConeLength || toolSet.isBetween(tankAngle - viewConeRadius / 4, tankAngle + viewConeRadius / 4, angleToPlayer) == false)
		{

			patrolMoveCooldown = maxPatrolMoveCooldown + 180;
			patrolGracePeriod = 60;

			enemyState = ENEMY_STATES::PATROL;
			patrolState = PATROL_STATES::SET_PATROL_MOVE_GOAL;

		}

		//If the enemy has not left the alert state by the time the grace period
		//reaches 0, enter combat
		if (alertGracePeriod <= 0)
		{

			enemyState = ENEMY_STATES::ALERT_COMBAT;

		}

	}
	else
	{
		if (isBoss == true)
		{
			enemyState = ENEMY_STATES::ALERT_COMBAT;
		}
		else
		{
			for (int i = 0; i < playerController->bulletController.bulletPool.size(); i++)
			{

				if (invunTime <= 0 && playerController->bulletController.bulletPool[i].IsAvailable() == false)
				{

					int bulletX = playerController->bulletController.bulletPool[i].x;
					int bulletY = playerController->bulletController.bulletPool[i].y;

					if (toolSet.Distance(x, y, bulletX, bulletY) < 45)
					{

						//Make the enemy enter combat
						enemyState = ENEMY_STATES::ALERT_COMBAT;

						//Decrease life
						playerController->bulletController.bulletPool[i].life = -1;
						life -= 1;
						invunTime = 60;

						//Fire particles
						particleController->AddParticle(x, y, 0, 0, 0, 40, 0.8, toolSet.RandomRange(120, 180), Color::White);

						for (int i = 0; i < 150; i++)
						{
							particleController->AddParticle(x, y, toolSet.RandomRange(0, 360), toolSet.RandomRange(5, 12), 0.9, toolSet.RandomRange(2, 7), 0.95, toolSet.RandomRange(120, 180), Color(255, 163, 0));
						}

					}

				}

			}
		}
	}

}

//Enemy default:
void EnemyDefault::Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController)
{

	//Initialise all pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;
	playerController = _playerController;
	roomController = _roomController;
	bulletController = _bulletController;
	particleController = _particleController;

	//Grab the spritesheet
	sprites = spriteLoader->GetTexture("spritesheet.png");

	//Set up the sprites
	IntRect rect(0, 0, 16, 16);
	base.setTextureRect(rect);
	base.setTexture(sprites);

	IntRect rect2(0, 16, 9, 16);
	tank.setTextureRect(rect2);
	tank.setTexture(sprites);

	tank.setScale(3, 3);
	base.setScale(3, 3);

	base.setOrigin(8, 8);
	tank.setOrigin(5, 11);

	//Set up the patrol variables
	patrolBasePosition = Vector2f(_x, _y);
	GenerateViewConeSprite();

	//Set the max fire cooldown
	maxFireCooldown = 120;

	//Set position
	x = _x;
	y = _y;

	//Set texture rect
	tankAnimRect.top = 0;
	tankAnimRect.left = 0;
	tankAnimRect.width = 16;
	tankAnimRect.height = 16;
	tankRectModifier = 16;

}

void EnemyDefault::Chase()
{

	//Move towards the player
	double distanceToPlayer = toolSet.Distance(x, y, playerController->player.x, playerController->player.y);
	double playerAngle = toolSet.AngleBetweenPoints(x, y, playerController->player.x, playerController->player.y);

	if (distanceToPlayer > 200 && fireCooldown > 0)
	{

		//Move the enemy
		MoveEnemy(playerAngle);
		
		//Point the base towards the player
		baseAngle = toolSet.RadiansToDegrees(playerAngle);
		base.setRotation(baseAngle + 90);

	}

	//Point the tank towards the player
	tankAngle = toolSet.bisectAngle(tankAngle, toolSet.RadiansToDegrees(playerAngle) + 90, 0.025);
	tank.setRotation(tankAngle);

	//Fire
	fireCooldown -= 1;

	if (fireCooldown <= 0)
	{
		
		if (bulletsToFire > 0)
		{

			individualFireCooldown -= 1;

			if (individualFireCooldown <= 0)
			{
				bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(tankAngle - 90), 4, 180);
				bulletsToFire -= 1;
				individualFireCooldown = 30;
			}
		}
		else
		{
			fireCooldown = maxFireCooldown;
			bulletsToFire = 3;
		}

	}

}

//Enemy sniper:
void EnemySniper::Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController)
{

	//Initialise all pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;
	playerController = _playerController;
	roomController = _roomController;
	bulletController = _bulletController;
	particleController = _particleController;

	//Grab the spritesheet
	sprites = spriteLoader->GetTexture("spritesheet.png");

	//Set up the sprites
	IntRect rect(0, 0, 16, 16);
	base.setTextureRect(rect);
	base.setTexture(sprites);

	IntRect rect2(9, 16, 11, 17);
	tank.setTextureRect(rect2);
	tank.setTexture(sprites);

	tank.setScale(3, 3);
	base.setScale(3, 3);

	base.setOrigin(8, 8);
	tank.setOrigin(6, 13);

	//Set up the patrol variables
	patrolBasePosition = Vector2f(_x, _y);
	GenerateViewConeSprite();

	//Set the max fire cooldown
	maxFireCooldown = 120;

	//Set position
	x = _x;
	y = _y;

	//Update the view cone length to match this new enemy
	viewConeLength = 400;
	maxFireCooldown = 180;

	//Set texture rect
	tankAnimRect.top = 0;
	tankAnimRect.left = 0;
	tankAnimRect.width = 16;
	tankAnimRect.height = 16;
	tankRectModifier = 16;
	
}

void EnemySniper::Chase()
{

	//Move towards the player
	double distanceToPlayer = toolSet.Distance(x, y, playerController->player.x, playerController->player.y);
	double playerAngle = toolSet.AngleBetweenPoints(x, y, playerController->player.x, playerController->player.y);

	if (distanceToPlayer > 200 && fireCooldown > 0)
	{

		//Move the enemy
		MoveEnemy(playerAngle);

		//Point the base towards the player
		baseAngle = toolSet.RadiansToDegrees(playerAngle);
		base.setRotation(baseAngle + 90);

	}

	//Point the tank towards the player
	tankAngle = toolSet.bisectAngle(tankAngle, toolSet.RadiansToDegrees(playerAngle) + 90, 0.025);
	tank.setRotation(tankAngle);

	//Fire
	fireCooldown -= 1;

	if (fireCooldown <= 0)
	{

		bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(tankAngle - 90), 8, 180);
		bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(tankAngle - 90), 6, 240);
		bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(tankAngle - 90), 4, 360);
		fireCooldown = maxFireCooldown;

	}

}

//Enemy spreader:
void EnemySpreader::Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController)
{

	//Initialise all pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;
	playerController = _playerController;
	roomController = _roomController;
	bulletController = _bulletController;
	particleController = _particleController;

	//Grab the spritesheet
	sprites = spriteLoader->GetTexture("spritesheet.png");

	//Set up the sprites
	IntRect rect(0, 0, 16, 16);
	base.setTextureRect(rect);
	base.setTexture(sprites);

	IntRect rect2(20, 20, 19, 12);
	tank.setTextureRect(rect2);
	tank.setTexture(sprites);

	tank.setScale(3, 3);
	base.setScale(3, 3);

	base.setOrigin(8, 8);
	tank.setOrigin(10, 6);

	//Set the max fire cooldown
	maxFireCooldown = 120;

	//Set position
	x = _x;
	y = _y;

	//Update the view cone length to match this new enemy
	viewConeLength = 100;
	viewConeRadius = 135;
	maxFireCooldown = 180;

	//Set up the patrol variables
	patrolBasePosition = Vector2f(_x, _y);
	GenerateViewConeSprite();

	//Set texture rect
	tankAnimRect.top = 0;
	tankAnimRect.left = 0;
	tankAnimRect.width = 16;
	tankAnimRect.height = 16;
	tankRectModifier = 16;

}

void EnemySpreader::Chase()
{

	//Move towards the player
	double distanceToPlayer = toolSet.Distance(x, y, playerController->player.x, playerController->player.y);
	double playerAngle = toolSet.AngleBetweenPoints(x, y, playerController->player.x, playerController->player.y);

	if (distanceToPlayer > 200 && fireCooldown > 0)
	{

		//Move the enemy
		MoveEnemy(playerAngle);

		//Point the base towards the player
		baseAngle = toolSet.RadiansToDegrees(playerAngle);
		base.setRotation(baseAngle + 90);

	}

	//Point the tank towards the player
	tankAngle = toolSet.bisectAngle(tankAngle, toolSet.RadiansToDegrees(playerAngle) + 90, 0.025);
	tank.setRotation(tankAngle);

	//Fire
	fireCooldown -= 1;

	if (fireCooldown <= 0)
	{

		bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(tankAngle - 90), 4, 180);
		bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(tankAngle - 90 + 45), 4, 180);
		bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(tankAngle - 90 - 45), 4, 180);

		fireCooldown = maxFireCooldown;

	}

}

//Enemy shield boss:
void EnemyShieldBoss::Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController, BackgroundController* _backgroundController)
{

	//Initialise all pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;
	playerController = _playerController;
	roomController = _roomController;
	bulletController = _bulletController;
	particleController = _particleController;
	backgroundController = _backgroundController;

	//Grab the spritesheet
	sprites = spriteLoader->GetTexture("spritesheet.png");

	//Set up the sprites
	IntRect rect(148, 50, 39, 39);
	base.setTextureRect(rect);
	base.setTexture(sprites);

	IntRect rect2(156, 89, 20, 20);
	tank.setTextureRect(rect2);
	tank.setTexture(sprites);

	tank.setScale(3, 3);
	base.setScale(3, 3);

	base.setOrigin(19.5, 19.5);
	tank.setOrigin(10, 10);

	//Set the max fire cooldown
	maxFireCooldown = 120;

	//Set position
	x = _x;
	y = _y;

	//Update the view cone length to match this new enemy
	viewConeLength = 100;
	viewConeRadius = 135;
	maxFireCooldown = 180;

	//Update boss hp
	life = maxLife;
	isMiniBoss = true;

	//Set up the patrol variables
	patrolBasePosition = Vector2f(_x, _y);
	GenerateViewConeSprite();

	//Create the render texture
	bossHP.create(130, 20);

	//Load music
	if (!miniBossMusic.openFromFile("MiniBoss.wav"))
	{
		toolSet.ConsoleOut("Couldn't load music.");
	}

}

void EnemyShieldBoss::Chase()
{

	//Stop music if player dead
	if (playerController->player.health <= 0)
	{
		miniBossMusic.stop();
	}

	//Change the background colour
	backgroundController->SetColour(Color(255, 0, 77));

	//Get the players angle
	double playerAngle = toolSet.AngleBetweenPoints(x, y, playerController->player.x, playerController->player.y);

	//Rotate the boss sprite
	tankAngle += 0.5;
	baseAngle = tankAngle / 2;
	base.setRotation(baseAngle);

	switch (attackState)
	{

		fireCooldown -= 1;

		case ATTACK_INTERVAL:

			//Randomise variables for next phase
			circleSize = toolSet.RandomRange(5, 15);
			circlesToFire = toolSet.RandomRange(3, 5);

			bulletsToFirePlayer = toolSet.RandomRange(5, 15);
			bulletsToFireCooldown = toolSet.RandomRange(15, 30);
			bulletPlayerSpeed = 1;

			attackState = SHIELD_ATTACKTYPE(rand() % ATTACK_INTERVAL);

		break;

		case ATTACK_CIRCLE:

			//Create bullets in a circle comprised of circleSize bullets
			//Bullets are evenly offset from each other

			//Fire circles of bullets with a cooldown
			if (circleFireCooldown <= 0)
			{

				for (int i = 1; i < circleSize+1; i++)
				{
					bulletController->AddNewBullet(x, y, toolSet.RadiansToDegrees(playerAngle) + (360 / circleSize) * i, 5, 60);
				}

				circlesToFire -= 1;
				circleFireCooldown = 60;

			}

			//If no bullets left to fire, return to interval state
			if (circlesToFire <= 0)
			{
				attackState = ATTACK_INTERVAL;
				circleFireCooldown = 120;
			}

			circleFireCooldown -= 1;

		break;

		case ATTACK_PLAYER_SPREAD:

			//Fire bullets at the player with a random spread
			if (bulletsToFireCooldown <= 0)
			{

				bulletsToFirePlayer -= 1;

				for (int i = 0; i < toolSet.RandomRange(1, 5); i++)
				{
					double bulletAngle = toolSet.DegreesToRadians(toolSet.RadiansToDegrees(angleToPlayer) + toolSet.RandomRange(-10, 10));
					bulletController->AddNewBullet(x, y, bulletAngle, toolSet.RandomRange(3, 7), toolSet.RandomRange(25, 75));
				}

				bulletsToFireCooldown = 20;

			}

			//If no bullets left to fire, return to interval statestate
			if (bulletsToFirePlayer <= 0)
			{
				attackState = ATTACK_INTERVAL;
				bulletsToFireCooldown = 120;
			}

			bulletsToFireCooldown -= 1;

		break;

		case ATTACK_PLAYER_TARGET:
		
			//Fire bullets at the player with a random spread
			if (bulletsToFireCooldown <= 0)
			{

				bulletsToFirePlayer -= 1;
				bulletPlayerSpeed += 1;

				bulletController->AddNewBullet(x, y, angleToPlayer, bulletPlayerSpeed, 120);

				bulletsToFireCooldown = 10;

			}

			//If no bullets left to fire, return to interval statestate
			if (bulletsToFirePlayer <= 0)
			{
				attackState = ATTACK_INTERVAL;
				bulletsToFireCooldown = 120;
			}

			bulletsToFireCooldown -= 1;

		break;
		

	}

	Sprite skull;
	IntRect skullRect(125, 0, 14, 15);
	skull.setTexture(sprites);
	skull.setTextureRect(skullRect);
	skull.setPosition(0, 0);

	bossHP.draw(skull);

	RectangleShape outline;
	outline.setOutlineThickness(1);
	outline.setOutlineColor(Color::White);
	outline.setFillColor(Color::Black);
	outline.setPosition(20, 2);
	outline.setSize(Vector2f(102, 10));

	RectangleShape inner;
	inner.setFillColor(Color::White);
	inner.setPosition(23, 4);
	inner.setSize(Vector2f((life/maxLife) * 100, 6));

	bossHP.draw(outline);
	bossHP.draw(inner);
	bossHP.display();

	//Generate and draw the sprite
	const Texture& texture = bossHP.getTexture();
	Vector2f viewPos = gameRenderer->GetViewPosition();
	Vector2f viewSize = gameRenderer->GetViewSize();

	bossBar.setTexture(texture);
	bossBar.setScale(3, 3);
	bossBar.setOrigin(65, 10);
	bossBar.setPosition(viewPos.x, viewPos.y + 250);

	gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX, bossBar);

}

//Enemy guard boss:
void EnemyGuardBoss::Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController, BackgroundController* _backgroundController)
{

	//Initialise all pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;
	playerController = _playerController;
	roomController = _roomController;
	bulletController = _bulletController;
	particleController = _particleController;
	backgroundController = _backgroundController;

	//Grab the spritesheet
	sprites = spriteLoader->GetTexture("spritesheet.png");

	//Set up the sprites
	IntRect rect(182, 0, 46, 31);
	base.setTextureRect(rect);
	base.setTexture(sprites);

	IntRect rect2(156, 0, 26, 37);
	tank.setTextureRect(rect2);
	tank.setTexture(sprites);

	tank.setScale(6, 6);
	base.setScale(6, 6);

	base.setOrigin(23.5, 15.5);
	tank.setOrigin(13.5, 27.5);

	//Set the max fire cooldown
	maxFireCooldown = 120;

	//Set position
	x = _x;
	y = _y;

	//Update boss variables
	life = maxLife;
	isBoss = true;
	collisionDistance = 180;

	//Set up the patrol variables
	patrolBasePosition = Vector2f(_x, _y);
	GenerateViewConeSprite();

	//Create the render texture
	bossHP.create(130, 20);

	//Set texture rect
	tankAnimRect.top = 0;
	tankAnimRect.left = 182;
	tankAnimRect.width = 46;
	tankAnimRect.height = 31;
	tankRectModifier = 46;

}

void EnemyGuardBoss::Chase()
{

	//Do checks for low health to enable overlay and alarm sound
	if (life <= 15)
	{
		if (playAlarm == true)
		{
			playAlarm = false;
			gameRenderer->ToggleAlarmOverlay();
		}
	}

	//Change the background colour
	backgroundController->SetColour(Color(255, 0, 77));

	//Get the players angle
	double playerAngle = toolSet.AngleBetweenPoints(x, y, playerController->player.x, playerController->player.y);
	
	//Rotate the tank
	tank.setRotation(toolSet.RadiansToDegrees(playerAngle) + 90);

	switch (attackState)
	{

	case BOSS_ATTACK_INTERVAL:
	{
		//Move the boss
		baseAngle = toolSet.RadiansToDegrees(toolSet.AngleBetweenPoints(x, y, goalX, goalY));

		movementCooldown -= 1;

		if (movementCooldown > 0)
		{

			int speed = 1;

			x += cos(toolSet.DegreesToRadians(baseAngle)) * speed;
			y += sin(toolSet.DegreesToRadians(baseAngle)) * speed;

			base.setRotation(baseAngle - 90);

		}
		else if(movementCooldown <=0)
		{

			//Randomise all firing variables and choose an attackstate
			circleSize = toolSet.RandomRange(5, 15);
			circlesToFire = toolSet.RandomRange(3, 5);

			bulletsToFire = toolSet.RandomRange(20, 50);
			bulletFireCooldown = toolSet.RandomRange(1, 5);
			maxBulletFireCooldown = bulletFireCooldown;
			bulletPlayerSpeed = 1;

			attackState = BOSS_ATTACKTYPE(rand() % BOSS_ATTACK_INTERVAL);

		}
		break;
	}
	case ATTACK_BASE_CIRCLE:
	{
		//Create bullets in a circle comprised of circleSize bullets
		//Bullets are evenly offset from each other

		//Fire circles of bullets with a cooldown
		if (circleFireCooldown <= 0)
		{

			for (int i = 1; i < circleSize + 1; i++)
			{

				//Work out position for angle offset
				int fireX = x + cos(toolSet.DegreesToRadians((360 / circleSize) * i)) * 150;
				int fireY = y + sin(toolSet.DegreesToRadians((360 / circleSize) * i)) * 150;
				double fireAngle = toolSet.AngleBetweenPoints(fireX, fireY, playerController->player.x, playerController->player.y);

				//Fire the bullets
				bulletController->AddNewBullet(fireX, fireY, fireAngle, 5, 180);

			}

			circlesToFire -= 1;
			circleFireCooldown = 60;

		}

		//If no bullets left to fire, return to interval state
		if (circlesToFire <= 0)
		{
			attackState = BOSS_ATTACK_INTERVAL;
			circleFireCooldown = 120;

			goalX = toolSet.RandomRange(256, 768);
			goalY = toolSet.RandomRange(256, 768);
			movementCooldown = maxMovementCooldown;

		}

		circleFireCooldown -= 1;

		break;
	}
	case ATTACK_BASE_RANDOM:
	{
		bulletFireCooldown -= 1;

		if (bulletFireCooldown <= 0)
		{

			//Create random bullets
			bulletController->AddNewBullet(x, y, toolSet.RadiansToDegrees(toolSet.RandomRange(0, 360)), toolSet.RandomRange(1, 3), toolSet.RandomRange(120, 180));

			bulletFireCooldown = maxBulletFireCooldown;
			bulletsToFire -= 1;

		}

		if (bulletsToFire <= 0)
		{
			attackState = BOSS_ATTACK_INTERVAL;

			goalX = toolSet.RandomRange(256, 768);
			goalY = toolSet.RandomRange(256, 768);
			movementCooldown = 180;
			movementCooldown = maxMovementCooldown;

		}

		break;
	}
	case ATTACK_BASE_SPIRAL:
	{
		bulletFireCooldown -= 1;

		if (bulletFireCooldown <= 0)
		{

			//Create random bullets
			bulletController->AddNewBullet(x, y, toolSet.RadiansToDegrees(spiralBulletAngle), 3, 120);

			bulletFireCooldown = maxBulletFireCooldown;
			bulletsToFire -= 1;

			spiralBulletAngle += 10;

		}

		if (bulletsToFire <= 0)
		{
			attackState = BOSS_ATTACK_INTERVAL;

			goalX = toolSet.RandomRange(256, 768);
			goalY = toolSet.RandomRange(256, 768);
			movementCooldown = 180;
			movementCooldown = maxMovementCooldown;

		}

		break;
	}
	case ATTACK_TANK_AIM:
	{
		//Fire bullets at the player with a random spread
		if (bulletFireCooldown <= 0)
		{

			bulletsToFire -= 1;
			bulletPlayerSpeed += 0.25;

			bulletController->AddNewBullet(x, y, playerAngle, bulletPlayerSpeed, 120);

			bulletFireCooldown = maxBulletFireCooldown;

		}

		//If no bullets left to fire, return to interval state
		if (bulletsToFire <= 0)
		{
			attackState = BOSS_ATTACK_INTERVAL;
			bulletFireCooldown = 120;

			goalX = toolSet.RandomRange(256, 768);
			goalY = toolSet.RandomRange(256, 768);
			movementCooldown = 180;
			movementCooldown = maxMovementCooldown;

		}

		bulletFireCooldown -= 1;

		break;
	}
	case ATTACK_TANK_SHOTGUN:
	{
		//Fire bullets at the player with a random spread
		if (bulletFireCooldown <= 0)
		{

			bulletsToFire -= 10;

			for (int i = 0; i < toolSet.RandomRange(3, 7); i++)
			{
				bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(toolSet.RadiansToDegrees(angleToPlayer) + toolSet.RandomRange(-30, 30)), toolSet.RandomRange(3, 7), toolSet.RandomRange(120, 180));
			}

			bulletFireCooldown = 30;

		}

		//If no bullets left to fire, return to interval state
		if (bulletsToFire <= 0)
		{
			attackState = BOSS_ATTACK_INTERVAL;
			bulletFireCooldown = 120;

			goalX = toolSet.RandomRange(256, 768);
			goalY = toolSet.RandomRange(256, 768);
			movementCooldown = 180;
			movementCooldown = maxMovementCooldown;

		}

		bulletFireCooldown -= 1;

		break;
	}
	case ATTACK_TANK_SMEAR:
	{
		//Fire bullets at the player with a random spread
		if (bulletFireCooldown <= 0)
		{

			bulletsToFire -= 10;
			bulletPlayerSpeed += 1;

			for (int i = 0; i < toolSet.RandomRange(3, 7); i++)
			{
				bulletController->AddNewBullet(x, y, toolSet.DegreesToRadians(angleToPlayer + toolSet.RandomRange(-30, 30)), toolSet.RandomRange(1, 3), toolSet.RandomRange(120, 180));
			}

			bulletFireCooldown = 1;

		}

		//If no bullets left to fire, return to interval state
		if (bulletsToFire <= 0)
		{
			attackState = BOSS_ATTACK_INTERVAL;
			bulletFireCooldown = 120;

			goalX = toolSet.RandomRange(256, 768);
			goalY = toolSet.RandomRange(256, 768);
			movementCooldown = 180;
			movementCooldown = maxMovementCooldown;

		}

		bulletFireCooldown -= 1;

		break;
	}

	}

	//Draw the health bar to the rendertexture
	Sprite skull;
	IntRect skullRect(125, 0, 14, 15);
	skull.setTexture(sprites);
	skull.setTextureRect(skullRect);
	skull.setPosition(0, 0);

	bossHP.draw(skull);

	RectangleShape outline;
	outline.setOutlineThickness(1);
	outline.setOutlineColor(Color::White);
	outline.setFillColor(Color::Black);
	outline.setPosition(20, 2);
	outline.setSize(Vector2f(102, 10));

	RectangleShape inner;
	inner.setFillColor(Color::White);
	inner.setPosition(23, 4);
	inner.setSize(Vector2f((life / maxLife) * 100, 6));

	bossHP.draw(outline);
	bossHP.draw(inner);
	bossHP.display();

	//Generate and draw the healthbar
	const Texture& texture = bossHP.getTexture();
	Vector2f viewPos = gameRenderer->GetViewPosition();
	Vector2f viewSize = gameRenderer->GetViewSize();

	bossBar.setTexture(texture);
	bossBar.setScale(3, 3);
	bossBar.setOrigin(65, 10);
	bossBar.setPosition(viewPos.x, viewPos.y + 250);

	gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX, bossBar);

}

//Reset the enemy controller
void EnemyController::Reset()
{

	//Destroy all enemies
	for (int i = 0; i < enemyPool.size(); i++)
	{
		enemyPool[i]->life = -1;
	}

	//Destroy all bullets
	bulletController.Destroy();

	//Reset variables
	playAlarm = true;
	endingSequence = false;
	enemyCount = 0;

	//Destroy all particles
	particleController.ClearAllParticles();

}