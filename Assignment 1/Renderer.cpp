//Renderer.cpp: This file is responsible for rendering of the game.
//

#include "Renderer.h"
#include <vector>


//Initialisation function
//Initialises the window and the view
void GameRenderer::Initialise(int _screenWidth = 1920, int _screenHeight = 1080)
{

	window.create(VideoMode(_screenWidth, _screenHeight), "Title");
	view.setSize(240, 135);
	view.zoom(5);
	view.setCenter(120, 68);

	//Resize the view
	view = getLetterboxView(view, _screenWidth, _screenHeight);

}

//source: https://stackoverflow.com/questions/30963204/c-vector-bubble-sort
vector<DrawableObject> BubbleSort(vector<DrawableObject> a)
{

	for (size_t i = 0; i < a.size() - 1; i++)
	{

		int depth = a[i].depth;
		int depth2 = a[i+1].depth;

		if (depth > depth2)
		{

			DrawableObject temp = a[i];
			a[i] = a[i + 1];
			a[i + 1] = temp;

		}

	}

	return a;

}

void GameRenderer::SetFrameRate(int _val)
{
	window.setFramerateLimit(_val);
}

bool GameRenderer::IsOpen()
{
	return window.isOpen();
}

void GameRenderer::Poll()
{

	//Event
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
		}

		if (event.type == sf::Event::Resized)
		{
			view = getLetterboxView(view, event.size.width, event.size.height);
		}
	}

}

void GameRenderer::Clear()
{
	window.clear();
}

void GameRenderer::Display()
{

	//zoom the view
	zoomFactor = 1;
	if (view.getSize().x < 1600 && Keyboard::isKeyPressed(Keyboard::Up))
	{
		zoomFactor = 1.1;
	}

	if (view.getSize().x > 990 && Keyboard::isKeyPressed(Keyboard::Down))
	{
		zoomFactor = 0.9;
	}
	view.zoom(zoomFactor);

	//sort through drawable objects
	if (!drawables.empty())
	{
		drawables = BubbleSort(drawables);
	}
	if (!guiDrawables.empty())
	{
		guiDrawables = BubbleSort(guiDrawables);
	}

	//draw the objects
	for (DrawableObject drawableObject : drawables)
	{
		window.draw(drawableObject.sprite);
	}

	//Draw the gui
	for (DrawableObject drawableObject : guiDrawables)
	{
		window.draw(drawableObject.sprite);
	}

	//Display overlay
	if (alarmOverlay == true)
	{

		//Create the shape
		RectangleShape overlay;
		overlay.setSize(Vector2f(5000, 5000));
		overlay.setPosition(-1000, -1000);

		//Set the colour
		alarmFadeAmount += 0.1;
		overlay.setFillColor(Color(255, 0, 77, 100 + sin(alarmFadeAmount) * alarmFadeIntensity));

		window.draw(overlay);

	}

	//displays screen
	window.display();
	
}

void GameRenderer::AddDrawable(double _depth, Sprite _sprite)
{

	DrawableObject object;

	object.depth = _depth;
	object.sprite = _sprite;

	drawables.push_back(object);

}

void GameRenderer::AddGuiDrawable(double _depth, Sprite _sprite)
{

	DrawableObject object;

	object.depth = _depth;
	object.sprite = _sprite;

	guiDrawables.push_back(object);

}

void GameRenderer::UpdateView()
{

	//shake the screen
	shakeAmount *= 0.95;

	shakeAmount = toolSet.RandomRange(-maxShakeAmount, maxShakeAmount);

	shakeX *= shakeAmount;
	shakeY *= shakeAmount;

	if (shakeAmount < 0.01)
	{
		shakeAmount = 0;
		maxShakeAmount = 0;
	}

	//update the view
	window.setView(view);

}

Vector2f GameRenderer::GetViewPosition()
{
	return view.getCenter();
}

Vector2f GameRenderer::GetViewSize()
{
	return view.getSize();
}

void GameRenderer::ViewSetFollowCoordinates(int _x, int _y)
{

	double x = view.getCenter().x;
	double y = view.getCenter().y;

	x = toolSet.Lerp(x, _x, 0.05);
	y = toolSet.Lerp(y, _y, 0.05);

	view.setCenter(x + shakeX, y + shakeY);
}

void GameRenderer::ScreenShake(double _fadeAmount, double _amount)
{

	shakeX = _amount;
	shakeY = _amount;
	maxShakeAmount = _amount;

}

//Resizes the view
sf::View GameRenderer::getLetterboxView(sf::View view, int windowWidth, int windowHeight) {

	// Compares the aspect ratio of the window to the aspect ratio of the view,
	// and sets the view's viewport accordingly in order to archieve a letterbox effect.
	// A new view (with a new viewport set) is returned.

	float windowRatio = windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	// If horizontalSpacing is true, the black bars will appear on the left and right side.
	// Otherwise, the black bars will appear on the top and bottom.

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	return view;
}

void GameRenderer::ToggleAlarmOverlay()
{
	alarmOverlay = !alarmOverlay;
}

void GameRenderer::ClearDrawables()
{
	//Clear the old data from the lists ready for the next frame
	drawables.clear();
	guiDrawables.clear();
}