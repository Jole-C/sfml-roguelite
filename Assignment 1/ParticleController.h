#pragma once

#include "SFML/Graphics.hpp"
#include "Enums.h"
#include "Renderer.h"
#include "GameObject.h"
#include "SpriteLoader.h"
#include "Tools.h"

class ParticleController;

class Particle : public GameObject
{
	//Methods
public:

	void Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader); //Initialise a blank particle
	void Set(double _x, double _y, double _angle, double _speed, double _speedDegradeFactor, double _scale, double _scaleDegradeFactor, double _life, Color _colour);
	bool IsAvailable(); //Returns true or false depending on the availability of the particle
	void Update(); //Updates a particle

	//Variables
public:

	double angle;
	double speed;
	double size;
	double life = -1;
	double scale;

	double speedDegradeFactor = 0;
	double scaleDegradeFactor = 0;

	Sprite sprite1;
	Sprite sprite2;
	Sprite chosenSprite;

	Color colour;

};

class ParticleController
{

	//Methods
public:

	void Initialise(int _size, GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader); //Initialises a new particle controller with _size particles
	void ClearAllParticles(); //Clears all particles
	void AddParticle(double _x, double _y, double _angle, double _speed, double _speedDegradeFactor, double _scale, double _scaleDegradeFactor, double _life, Color _colour); //Add a particle if it is available
	void Update(); //Update all unavailable particles

	//Variables
public:

	vector<Particle> particlePool; //Pool of particles

private:

	Tools toolSet;

};

