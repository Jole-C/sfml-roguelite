// BulletController.cpp : Defines a bullet controller with it's own pool of bullets which can then be utilised by the objects calling them. Can also specify a class to detect collisions against.

#include "BulletController.h"


//Initialises a brand new bullet controller
void BulletController::Initialise(int _size, GameObject* _collisionObject, SpriteLoader* _spriteLoader, GameRenderer* _gameRenderer, RoomController* _roomController)
{

	//populate the array with bullets
	for (int i = 0; i < _size; i++)
	{
		Bullet bullet;
		bulletPool.push_back(bullet);
	}

	//assign objects
	collisionObject = _collisionObject;
	spriteLoader = _spriteLoader;
	gameRenderer = _gameRenderer;
	roomController = _roomController;

}

void BulletController::InitialiseBullets(Vector2f _spriteCoordinates, Vector2f _spriteSize, bool _followAngle)
{

	for (int i = 0; i < bulletPool.size(); i++)
	{
		bulletPool[i].Initialise(_spriteCoordinates, _spriteSize, _followAngle, spriteLoader, gameRenderer);
	}

}

//Check all bullets for availability
void BulletController::AddNewBullet(double _x, double _y, double _angle, double _speed, double _life)
{

	for (int i = 0; i < bulletPool.size(); i++)
	{

		//If bullet is available, set it to the new paramaters, then break out of the loop
		if (bulletPool[i].IsAvailable())
		{
			bulletPool[i].Set(_x, _y, _angle, _speed, _life);
			return;
		}

	}

}

//Updates the bullet controller
void BulletController::Update()
{

	for (int i = 0; i < bulletPool.size(); i++)
	{

		if (bulletPool[i].IsAvailable() == false)
		{

			//Update the bullet
			bulletPool[i].Update();

			//Check for wall collisions
			int bulletX = bulletPool[i].x;
			int bulletY = bulletPool[i].y;
			int bulletTile = roomController->GetTile(bulletX, bulletY);

			if (!(bulletTile == 45 || bulletTile == 5) || (bulletX < roomController->MIN_ROOM_POS_X || bulletX > roomController->MAX_ROOM_POS_X || bulletY < roomController->MIN_ROOM_POS_Y || bulletY > roomController->MAX_ROOM_POS_Y))
			{
				bulletPool[i].life = -1;
			}

		}
	
	}
}

//Destroys all bullets
void BulletController::Destroy()
{

	for (int i = 0; i < bulletPool.size(); i++)
	{
		bulletPool[i].life = -1;
	}

}


//Bullet logic

//Initialises a brand new blank bullet
void Bullet::Initialise(Vector2f _spriteCoordinates, Vector2f _spriteSize, bool _followAngle, SpriteLoader* _spriteLoader, GameRenderer* _gameRenderer)
{

	//Set objects and followangle variable
	spriteLoader = _spriteLoader;
	gameRenderer = _gameRenderer;
	followAngle = _followAngle;

	//Initialise some variables for later
	IntRect textureRect;

	//Set the texture rect to the arguments
	textureRect.top = _spriteCoordinates.y;
	textureRect.left = _spriteCoordinates.x;
	textureRect.width = _spriteSize.x;
	textureRect.height = _spriteSize.y;

	//Load the texture
	sprites = spriteLoader->GetTexture("spritesheet.png");

	//Assign the above variables to the sprite
	sprite.setTexture(sprites);
	sprite.setTextureRect(textureRect);

	//Set up the specific sprite paramaters
	int spriteX = _spriteSize.x / 2;
	int spriteY = _spriteSize.y / 2;

	sprite.setOrigin(spriteX, spriteY);
	sprite.setScale(3, 3);

	depth = DEPTH_GAME_MAX - 1;

}

//Returns true or false depending on whether the life of the bullet is less than 0
bool Bullet::IsAvailable()
{
	if (life < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Updates a bullet
void Bullet::Update()
{

	//Only execute code if life is greater than 0
	life -= 1;

	if (life > 0)
	{

		//move the bullet
		x += cos(angle) * speed;
		y += sin(angle) * speed;

		//If follow angle is enabled, rotate the bullet
		if (followAngle == true)
		{
			sprite.setRotation(toolSet.RadiansToDegrees(angle) + 90);
		}
		else
		{
			sprite.setRotation(0);
		}

		//Push the sprite into the render queue
		sprite.setPosition(x, y);
		gameRenderer->AddDrawable(depth, sprite);

	}
		
}

//Sets a bullet to the given paramaters, only called if the bullet is available for use (E.G dead)
void Bullet::Set(double _x, double _y, double _angle, double _speed, double _life)
{

	x = _x;
	y = _y;
	angle = _angle;
	speed = _speed;
	life = _life;

}