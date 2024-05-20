#pragma once

#include "Renderer.h";
#include "GameObject.h";
#include "RoomController.h";
#include "BulletController.h";
#include "ParticleController.h";
#include "TextController.h";
#include "SpriteLoader.h";
#include "Enums.h";
#include "Tools.h";
#include "BackgroundController.h"
#include "SFML/Audio.hpp"

class EnemyController;
class PlayerController;

class EnemyBase : public GameObject
{

	//Methods
public:

	virtual void Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController) {}; //Initialises a new enemy
	void GenerateViewConeSprite(); //Generates the view cone sprite
	void Update(); //Updates the enemy
	void MoveEnemy(double _playerAngle); //Moves the enemy, including collision checks
	virtual void Patrol(); //Default patrol behaviour, shared across enemies
	virtual void Alert();
	virtual void Chase() { toolSet.ConsoleOut("I am the enemy base, executing its chasing code. This should not happen."); };
	virtual void Death() { toolSet.ConsoleOut("I am the enemy base, executing its death code. This should not happen."); };
	
	//Variables
public:

	//Firing variables
	double fireCooldown;
	double maxFireCooldown;

	//Life variables
	double life = 2;
	int invunTime;
	int collisionDistance = 75;

	//Patrol behaviour
	int patrolRadius = 80;
	int patrolMoveCooldown = 180;
	int maxPatrolMoveCooldown = 180;
	int patrolSpeed = 3;
	double patrolCannonAngle;
	double angleToPlayer;
	bool permanentCombat = false;
	bool isBoss = false;
	bool isMiniBoss = false;

	int viewConeRadius = 90;
	int viewConeLength = 250;
	int hearingRadius = 210;

	//Alert behaviour
	int alertGracePeriod = 180;
	int patrolGracePeriod = 180;
	int minimumAlertPeriod = 120;

	Vector2f patrolBasePosition;
	Vector2f targetPatrolPosition;

	//Enemy behaviour
	ENEMY_STATES enemyState = ENEMY_STATES::PATROL;
	PATROL_STATES patrolState = PATROL_STATES::SET_PATROL_MOVE_GOAL;
	ENEMY_TYPE enemyType;

	//Sprites
	Sprite base;
	Sprite tank;
	Sprite viewCone;
	Sprite viewConeSprite;
	RenderTexture viewConeTexture;
	double baseAngle;
	double tankAngle;

	//Pointers
	GameRenderer* gameRenderer;
	SpriteLoader* spriteLoader;
	PlayerController* playerController;
	BulletController* bulletController;
	RoomController* roomController;
	ParticleController* particleController;

	//Objects
	Tools toolSet;

	//Music
	bool playMusic = true;
	Music miniBossMusic;

	//Tank animation
	IntRect tankAnimRect;
	int tankRectModifier = 16;
	double tankFrame = 0;

};

class EnemyDefault : public EnemyBase
{

	//Methods
public:

	void Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController);
	void Chase();

	//Variables
private:

	int bulletsToFire = 3;
	double individualFireCooldown = 30;

};

class EnemySniper : public EnemyBase
{

	//Methods
public:

	void Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController);
	void Chase();

	//Variables
private:

};

class EnemySpreader : public EnemyBase
{

	//Methods
public:

	void Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController);
	void Chase();

	//Variables
private:

};

class EnemyShieldBoss : public EnemyBase
{

	//Methods
public:

	void Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController, BackgroundController* _backgroundController);
	void Chase();

	//Variables
private:

	bool permanentCombat = true;
	SHIELD_ATTACKTYPE attackState = ATTACK_INTERVAL;

	//Circle attack vars
	int circleSize;
	int circlesToFire;
	int circleFireCooldown;

	//Player spread and target
	int bulletsToFirePlayer;
	int bulletsToFireCooldown;
	int bulletPlayerSpeed;

	//Background manipulation
	BackgroundController* backgroundController;

	Sprite bossBar;
	RenderTexture bossHP;

	double maxLife = 18;

};

class EnemyGuardBoss : public EnemyBase
{

	//Methods
public:

	void Initialise(int _x, int _y, GameRenderer* _gameRenderer, PlayerController* _playerController, SpriteLoader* _spriteLoader, RoomController* _roomController, BulletController* _bulletController, ParticleController* _particleController, BackgroundController* _backgroundController);
	void Chase();

	//Variables
private:

	bool permanentCombat = true;
	BOSS_ATTACKTYPE attackState = BOSS_ATTACK_INTERVAL;

	//Background manipulation
	BackgroundController* backgroundController;

	Sprite bossBar;
	RenderTexture bossHP;

	double maxLife = 30;

	//Fire bullets in a circle at the player
	int circleSize;
	int circlesToFire;
	int circleFireCooldown;

	//Firing cooldowns
	int bulletsToFire;
	int bulletFireCooldown;
	int maxBulletFireCooldown;
	double bulletPlayerSpeed = 1;
	int spiralBulletAngle;

	//Movement variables
	int goalX = 512;
	int goalY = 512;
	int movementCooldown = 0;
	int maxMovementCooldown = 60;

	//Alarm sound
	bool playAlarm = true;


};

class EnemyController
{

	//Methods
public:

	void Initialise(GameRenderer* _renderer, SpriteLoader* _spriteLoader, PlayerController* _playerController, RoomController* _roomController, TextController* _textController, BackgroundController* _backgroundController);
	void AddEnemy(int _x, int _y, int _patrolRadius, ENEMY_TYPE _enemyType);
	void Update();
	void Reset();
	void DestroyAllEnemies();

	//Variables
public:

	int enemyCount;
	int enemyAlertRadius = 64; //If an enemy is close to another enemy that is in combat, make it alert

	vector<EnemyBase*> enemyPool;
	
	GameRenderer* gameRenderer;
	SpriteLoader* spriteLoader;
	PlayerController* playerController;
	BulletController bulletController;
	RoomController* roomController;
	ParticleController particleController;
	TextController* textController;
	BackgroundController* backgroundController;
	Tools toolSet;

	Music alarm;
	bool playAlarm = true;
	bool endingSequence = false;

};
