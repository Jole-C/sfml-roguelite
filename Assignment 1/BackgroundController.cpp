#include "BackgroundController.h"

void BackgroundController::Initialise(GameRenderer* _gameRenderer, SpriteLoader* _spriteLoader)
{

	//Set pointers
	gameRenderer = _gameRenderer;
	spriteLoader = _spriteLoader;

	//Set up the rendertexture
	sprites = spriteLoader->GetTexture("spritesheet.png");
	Sprite sprite;
	sprite.setTextureRect(IntRect(0, 48, 22, 22));
	sprite.setTexture(sprites);

	renderTexture.create(22, 22);
	renderTexture.draw(sprite);

}

void BackgroundController::Update()
{

	//Get the texture from the render texture
	texture = renderTexture.getTexture();
	texture.setRepeated(true);

	//Set the background texturerect
	background.setTextureRect(IntRect(0, 0, 5000, 3000));
	background.setTexture(texture);

	//Set the position offset from the mouse
	Vector2i mousePos = Mouse::getPosition();

	background.setColor(backgroundColour);
	background.setPosition(-1300 + mousePos.x / 16, -600 + mousePos.y / 16);

	gameRenderer->AddDrawable(DEPTH_BACKGROUND_MIN, background);


}

void BackgroundController::SetColour(Color _colour)
{
	//Set the background colour
	backgroundColour = _colour;
}
