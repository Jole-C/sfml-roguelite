#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Enums.h"
#include "Tools.h"
#include <iostream>
#include <deque>
#include <sstream>

using namespace std;
using namespace sf;

class TextBox
{

	//Variables
public:

	//Transition Variables
	TEXTBOX_STATE state = TEXTBOX_INTRO;

	//String to be used
	string text;

	//Display variables
	int x;
	int y = -310;
	bool skipOutro = false;
	bool skipIntro = false;

};

class TextController
{

	//Methods
public:

	void Initialise(GameRenderer* _gameRenderer); //Initialises the text controller
	void AddNewTextBox(string _text); //Adds a new text box and pushes it back to the text controller
	void Dump(); //Dumps all text boxes
	void Update(); //Updates the current text box, handling text display, transitions (intro/outro), etc
	bool IsEmpty(); //Returns true or false depending on whether any text boxes are currently dsplayed

private:


private:

	//Font and vector of text boxes
	Font textFont;
	deque<TextBox> textBoxes; //Vector of all text boxes and their text objects

	//Tool set
	Tools toolSet;

	//Display variables
	RenderTexture textTexture;
	GameRenderer* gameRenderer;

	//Text scroll variables
	string currentString;
	string currentStringShown;
	stringstream textScroller;
	Text text;
	TextBox currentBox;

	int currentLine = 0;
	int currentChar = 0;
	int maxFontSize = 20;
	int charLineLimit = 50;
	int maxLineLimit = 5;
	int timeUntilNextBox = 180;

};

