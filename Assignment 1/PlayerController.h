#pragma once

#include <SFML/Graphics.hpp>
#include "Enums.h"
#include "GameObject.h"
#include "Renderer.h"
#include "RoomController.h"
#include "BulletController.h"
#include "ParticleController.h"

class PlayerController;
class EnemyController;

class Player : public GameObject
{

	//Methods
public:

	void Initialise(PlayerController* _playerController, GameRenderer* _renderer, RoomController* _roomController, SpriteLoader* _spriteLoader, BulletController* _bulletController, ParticleController* _particleController, int _x, int _y);
	void Update(); //Updates the player
	void Reset(); //Resets player variables

private:

	//Variables
public:

	Sprite meleeWeapon; //The melee weapon

	int bulletCount = 5;
	int maxBulletCount = 5;

	//Firing variables
	double fireCooldown = 45;
	double maxFireCooldown = 45;
	int bulletRechargeGracePeriod = 70;
	int maxBulletRechargeGracePeriod = 70;
	double bulletRechargeRate = 30;
	double maxBulletRechargeRate = 30;

	//Health
	int deathState = 0;
	double health = 3;
	double maxHealth = 3;
	int invunTime = 60;
	int spawnCooldown = 60;

	//Movement
	int baseSpeed = 3;

	//Weapon variables
	int playerWeapon = 0;

	//Melee weapon
	int meleeCooldown = 30;
	int maxMeleeCooldown = 30;

	//Player's overlay sprite
	Texture overlayTexture; //The texture for the overlay sprite
	Sprite overlay;

private:

	Texture* spritesheetTexture; //The texture to grab the sprites from
	Sprite overlaySprite; //The overlay sprite
	Sprite playerBase; //The tank base
	Sprite playerCannon; //The player cannon

	PlayerController* playerController;
	BulletController* bulletController = new BulletController;
	RoomController* roomController;
	ParticleController* particleController;

	//Movement and aiming variables
	double aimDirection;
	double cannonDirection;
	double xSpeed;
	double ySpeed;
	
};


class PlayerController
{

	//Methods
public:

	void Initialise(GameRenderer* _renderer, RoomController* _roomController, SpriteLoader* _spriteLoader, int _x, int _y); //Core method - Initialises a new player controller
	void Update();
	void Reset();

	//Variables
public:

	Texture spritesheetTexture;
	SpriteLoader* spriteLoader;
	Player player;
	GameRenderer* gameRenderer;
	RoomController* roomController;
	Tools toolSet;
	BulletController bulletController;
	ParticleController particleController;
	EnemyController* enemyController;

	//For death reset
	int deathCooldown = 180;

private:

	//For GUI
	Sprite healthOutline;
	Sprite healthInner;
	Sprite bulletEmpty;
	Sprite bulletFull;
	Sprite playerWeapon;
	IntRect playerWeaponRect;

};

