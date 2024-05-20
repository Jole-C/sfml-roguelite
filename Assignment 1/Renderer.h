#pragma once

#include <SFML/Graphics.hpp>
#include "Tools.h"

using namespace sf;

class GameObject;

class DrawableObject
{
public:
	double depth;
	Sprite sprite;
};


class GameRenderer
{
	//Methods
public:

	void Initialise(int _screenWidth, int _screenHeight); //Initialises camera, display, etc
	bool IsOpen(); //Returns true if window is open
	void Poll(); //Polls for events
	void Clear(); //Clears the screen
	void Display(); //Displays the screen
	void UpdateView(); //Updates the view
	Vector2f GetViewPosition(); //Gets the view position
	Vector2f GetViewSize(); //Gets the view size
	void AddDrawable(double depth, Sprite sprite); //Adds depth and a sprite to the render queue
	void AddGuiDrawable(double _depth, Sprite _sprite);
	void SetFrameRate(int _val); //Sets the frame rate
	void ViewSetFollowCoordinates(int _x, int _y); //Makes the camera follow the given coordinates
	void ScreenShake(double _fadeAmount, double _amount); //Shake the screen by an amount
	sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight); //Used for aspect ratio fix
	void ToggleAlarmOverlay(); //Used for boss and ending
	void ClearDrawables(); //Clears the depth sorting vectors for the next frame, useful if you want a frame to freeze
	
private:

	//Variables
public:

	RenderWindow window; //window
	View view; //camera
	Event event; //event
	Tools toolSet; //set of useful methods
	vector<DrawableObject> drawables; //drawable objects
	vector<DrawableObject> guiDrawables;

private:

	Vector2f cameraTargetPosition;
	double shakeAmount;
	double maxShakeAmount;
	double shakeX;
	double shakeY;
	
	bool alarmOverlay = false;
	double alarmFadeAmount = 0;
	double alarmFadeIntensity = 50;

	double zoomFactor = 1;

};

