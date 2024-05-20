#pragma once

#include "GameObject.h"
#include "Enums.h"
#include "Renderer.h"
#include "SFML/Graphics.hpp"
#include "SpriteLoader.h"
#include "RoomController.h"

class BulletController;
class Tools;

//Simply moves in direction with speed
class Bullet : public GameObject
{

	//Methods
public:

	void Initialise(Vector2f _spriteCoordinates, Vector2f _spriteSize, bool _followAngle, SpriteLoader* _spriteLoader, GameRenderer* _gameRenderer); //Initialises a brand new blank bullet
	bool IsAvailable(); //Returns a bool for whether the bullet is available or not
	void Update(); //Updates a bullet
	void Set(double _x, double _y, double _angle, double _speed, double _life); //Sets the bullet to the given paramaters, to the player this seems like a new bullet


	//Variables
public:

	//Movement variables
	double speed;
	double angle;
	double life = -1;

private:

	
	Sprite sprite;
	bool followAngle; //Whether the bullet should rotate to face the direction of movement

};


class BulletController
{

	//Methods
public:

	void Initialise(int _size, GameObject* _collisionObject, SpriteLoader* _spriteLoader, GameRenderer* _gameRenderer, RoomController* _roomController); //Initialises a brand new bullet controller
	void InitialiseBullets(Vector2f _spriteCoordinates, Vector2f _spriteSize, bool _followAngle);
	void Update(); //Updates the bullet controller
	void AddNewBullet(double _x, double _y, double _angle, double _speed, double _life); //Adds a new bullet at the given position
	void Destroy(); //Destroys the bullet controller and all bullets

	//Variables
public:

	vector<Bullet> bulletPool; //The pool of bullets

private:


	//Other objects
	GameObject* collisionObject; //The object for collision
	SpriteLoader* spriteLoader; //For loading the spritesheet
	GameRenderer* gameRenderer; //For loading the game renderer
	RoomController* roomController; //For collision with walls
	Tools toolSet; //For tools

};

