//TextController.cpp : This class handles creation and displaying of text data in a text box.
//

#include "TextController.h"

using namespace sf;

void TextController::Initialise(GameRenderer* _gameRenderer)
{

	//Load the font
	if (!textFont.loadFromFile("Aquarius-RegularMono.ttf"))
	{
		toolSet.ConsoleOut("Failed to load font!");
		exit(1);
	}

	//Get a pointer to the game renderer
	gameRenderer = _gameRenderer;

	//Initialise the rendertexture
	textTexture.create(950, 160);

}

void TextController::AddNewTextBox(string _text)
{

	TextBox textBox;

	textBox.text = _text;
	textBoxes.push_back(textBox);


	//If there is only one text box in the system (EG, the one that was just added), set it to the currentBox
	if (textBoxes.size() == 1)
	{
		currentBox = textBoxes.front();
	}

}

void TextController::Dump()
{
	//Dump all text boxes out of the system
	textBoxes.clear();
}

void TextController::Update()
{

	//While we have text boxes to work with
	if(!textBoxes.empty())
	{

		//Grab the current text box state and initialise all necessary variables
		TEXTBOX_STATE currentBoxState = currentBox.state;
		
		//Set up the text
		text.setFont(textFont);
		text.setCharacterSize(maxFontSize);

		//Create rectangle for rendering
		RectangleShape box;
		box.setOutlineColor(Color::White);
		box.setFillColor(Color::Black);
		box.setOutlineThickness(5);
		box.setSize(Vector2f(935, 150));
		box.setPosition(5, currentBox.y + 5);

		switch (currentBoxState)
		{

			case TEXTBOX_INTRO:
			{
				
				//Simple scroll up intro.
				if (currentBox.y < 0 || currentBox.skipIntro == true)
				{
					currentBox.y += 10;
				}
				else
				{

					//Reset all variables for the upcoming update state
					currentLine = 0;
					currentChar = 0;
					timeUntilNextBox = 180;
					currentBox.state = TEXTBOX_UPDATING;
					currentStringShown.clear();
					currentString.clear();
					text.setString("");

					textScroller.str(currentBox.text);

				}

				//Replace the textbox in the vector with the modified one
				textBoxes.front() = currentBox;

				break;
			}


			case TEXTBOX_UPDATING:
			{

				//Get the current word and add it to the current string
				if (currentChar + (currentLine * charLineLimit) == currentString.length())
				{

					string word;
					getline(textScroller, word, ' ');

					//If the word is empty then getline has reached the end of the string.
					//If this happens, go to the outro state.
					if (!word.empty())
					{

						//If the word added will surpass the size of currentLine * charLineLimit, add a newline
						if ((currentLine + 1) * charLineLimit <= currentString.length() + word.length())
						{

							currentString += '\n';
							currentLine += 1;
							currentChar = 0;

						}

						currentString += word + ' ';

						//Check to see if this string would overlap the bottom of the box
						int maxLineSize = charLineLimit * 5;

						//If so, make a new box with the rest of the string
						if (currentString.length() > maxLineSize)
						{

							TextBox textBox;
							string restOfText;
							string currentWord;

							//Get the rest of the text in the string and push it back into the restoftext string
							restOfText += word + ' ';

							while (getline(textScroller, currentWord, ' '))
							{
								restOfText += currentWord + ' ';
							}

							textBox.text = restOfText;


							//Depending on how many boxes are in the queue, either push to the back safely,
							//or insert behind the box at the front

							if (textBoxes.size() == 1)
							{
								textBoxes.push_back(textBox);
							}
							else
							{
								textBoxes.insert(textBoxes.begin() + 1, textBox);
							}

							currentBox.state = TEXTBOX_OUTRO;

						}

					}
					else
					{
						currentBox.state = TEXTBOX_OUTRO;
					}

				}
				
				//Increment currentchar and use that to get the substring
				if (currentChar + (currentLine * charLineLimit) < currentString.length())
				{
					currentChar += 1;
				}

				string currentStringShown = currentString.substr(0, currentChar + currentLine * charLineLimit);

				text.setString(currentStringShown);

				//Replace the textbox in the vector with the modified one
				textBoxes.front() = currentBox;

				break;

			}


			case TEXTBOX_OUTRO:
			{

				//Do the box outro and then pop it after the cooldown
				timeUntilNextBox -= 1;

				if (timeUntilNextBox <= 0)
				{

					if (currentBox.y < 250)
					{

						currentBox.y += 10;

						//Replace the textbox in the vector with the modified one
						textBoxes.front() = currentBox;

					}
					else
					{

						currentString = "";
						currentStringShown = "";
						textBoxes.pop_front();
						textScroller.str("");
						textScroller.clear();

						if (textBoxes.size() > 0)
						{
							currentBox = textBoxes.front();
						}

					}

				}

				break;

			}


		}

		//Render the text box
		const Texture& texture = textTexture.getTexture();
		Sprite textSprite;
		
		text.setPosition(7, currentBox.y + 5);

		textTexture.clear(Color::Transparent);
		textTexture.draw(box);

		if (currentBox.state == TEXTBOX_UPDATING || currentBox.state == TEXTBOX_OUTRO)
		{
			textTexture.draw(text);
		}

		textSprite.setTexture(texture);
		textSprite.setScale(1, -1);

		textSprite.setOrigin(textSprite.getGlobalBounds().width / 2, textSprite.getGlobalBounds().height / 2);

		textSprite.setPosition(gameRenderer->GetViewPosition().x, gameRenderer->GetViewPosition().y + 175);

		gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX + 10, textSprite);

	}

}

bool TextController::IsEmpty()
{
	return textBoxes.empty();
}