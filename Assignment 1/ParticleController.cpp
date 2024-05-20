// ParticleController.cpp : This class controls visual effects with particles. Uses an object pool for optimisation.
#include "ParticleController.h"


void Particle::Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader)
{
	
	//Set up object pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;

	//Create sprites
	sprites = spriteLoader->GetTexture("spritesheet.png");
	IntRect rect;

	rect.width = 7;
	rect.height = 7;

	rect.top = 0;
	rect.left = 72;

	sprite1.setTexture(sprites);
	sprite1.setTextureRect(rect);

	rect.top = 0;
	rect.left = 79;

	sprite2.setTexture(sprites);
	sprite2.setTextureRect(rect);

	sprite1.setOrigin(4, 4);
	sprite2.setOrigin(4, 4);

}

void Particle::Set(double _x, double _y, double _angle, double _speed, double _speedDegradeFactor, double _scale, double _scaleDegradeFactor, double _life, Color _colour)
{

	//Normalise angle
	if (_angle > 360) _angle -= 360;
	if (_angle < 0) _angle += 360;

	//Set variables
	x = _x;
	y = _y;
	angle = toolSet.DegreesToRadians(_angle);
	speed = _speed;
	speedDegradeFactor = _speedDegradeFactor;
	scale = _scale;
	scaleDegradeFactor = _scaleDegradeFactor;
	life = _life;
	colour = _colour;

	int randChance = toolSet.RandomRange(1, 3);
	sprite1.setTexture(sprites);
	sprite2.setTexture(sprites);

	chosenSprite = sprite2;
	chosenSprite.setTexture(sprites);

	if (randChance == 1)
	{
		chosenSprite = sprite1;
	}

}

bool Particle::IsAvailable()
{
	return life <= 0;
}

void Particle::Update()
{

	//Decrease life
	if (life > 0)
	{

		life -= 1;

		//Move the particle
		x += cos(angle) * speed;
		y += sin(angle) * speed;

		speed *= speedDegradeFactor;

		chosenSprite.setPosition(x, y);

		//Scale it
		scale *= scaleDegradeFactor;
		chosenSprite.setScale(scale, scale);

		//Push to the renderqueue
		chosenSprite.setColor(colour);
		gameRenderer->AddDrawable(DEPTH_GAME_MAX, chosenSprite);

		//Ensure the particle dies once no longer visible
		if (scale <= 1)
		{
			life = -1;
		}

	}

}

void ParticleController::Initialise(int _size, GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader)
{

	for (int i = 0; i < _size; i++)
	{

		Particle particle;

		particle.Initialise(_gameRenderer, _spriteLoader);
		particlePool.push_back(particle);

	}

}

void ParticleController::ClearAllParticles()
{

	for (int i = 0; i < particlePool.size(); i++)
	{
		particlePool[i].life = -1;
	}

}

void ParticleController::AddParticle(double _x, double _y, double _angle, double _speed, double _speedDegradeFactor, double _scale, double _scaleDegradeFactor, double _life, Color _colour)
{

	for (int i = 0; i < particlePool.size(); i++)
	{

		if (particlePool[i].IsAvailable() == true)
		{
			particlePool[i].Set(_x, _y, _angle, _speed, _speedDegradeFactor, _scale, _scaleDegradeFactor, _life, _colour);
			break;
		}

	}

}

void ParticleController::Update()
{

	for (int i = 0; i < particlePool.size(); i++)
	{
		if (particlePool[i].IsAvailable() == false)
		{
			particlePool[i].Update();
		}
	}

}