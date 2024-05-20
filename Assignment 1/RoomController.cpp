// RoomController.cpp: Handles map and room generation.
//

#include "RoomController.h"
#include "SFML/Graphics.hpp"
#include "EnemyController.h"
#include <sstream>
#include <deque>
#include <math.h>
#include <random>

//room controller methods
void RoomController::Initialise(GameRenderer* _renderer, SpriteLoader* _spriteLoader)
{

	//Initialise the room vector
	currentRoom.setPrimitiveType(Quads);

	//Create a vector of RoomOjects that contains all necessary room info for each map screen (tile data & enemy spawn positions)
	//Tile 5 is used to indicate places where things can spawn
	//Tile 45 is used to indicate a completely blank tile
	RoomObjects roomObject;
	roomObject.roomString = "24,23,23,23,23,23,23,23,23,23,23,23,23,15,5,5,5,5,17,23,23,23,23,23,23,23,23,23,23,23,23,27,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,16,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,16,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,14,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,14,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,25,23,23,23,23,23,23,23,23,23,23,23,23,15,5,5,5,5,17,23,23,23,23,23,23,23,23,23,23,23,23,26;";
	roomStringObjects.push_back(roomObject);

	roomObject.roomString = "1,2,2,2,2,2,2,2,2,2,2,2,2,3,5,5,5,5,1,2,2,2,2,2,2,2,2,2,2,2,2,3,4,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,4,5,5,5,5,5,5,5,5,5,5,5,5,6,4,5,5,5,5,5,5,5,5,5,5,5,5,10,3,5,5,1,11,5,5,5,5,5,5,5,5,5,5,5,5,6,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,4,5,5,5,5,13,8,8,8,8,8,8,8,8,9,5,5,7,8,8,8,8,8,8,12,5,5,5,5,5,5,6,4,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,5,5,5,5,5,5,6,4,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,5,5,5,5,5,5,6,4,5,13,8,8,9,5,5,17,23,23,39,40,23,15,5,5,17,23,39,2,3,5,5,4,5,5,5,5,5,5,6,4,5,6,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,4,5,6,5,5,4,5,5,5,5,5,5,6,4,5,6,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,4,5,6,5,5,4,5,5,5,5,5,5,6,4,5,6,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,4,5,6,5,5,4,5,5,5,5,5,5,6,4,5,6,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,38,8,9,5,5,7,8,12,5,5,5,5,6,7,8,35,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,22,5,5,5,5,5,5,4,5,5,13,8,9,5,5,16,5,5,5,5,5,5,5,5,7,9,5,5,5,5,5,5,16,5,5,5,5,5,5,7,8,8,9,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,14,5,5,5,5,5,5,5,5,1,3,5,5,5,5,5,5,14,5,5,5,5,5,5,1,2,2,3,5,5,1,2,36,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,22,5,5,5,5,5,5,4,5,5,10,2,3,4,5,6,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,44,15,5,5,5,5,17,18,5,5,5,5,6,4,5,6,5,5,5,5,5,5,5,5,4,20,23,15,5,5,17,23,42,5,5,5,5,5,5,4,5,5,5,5,6,4,5,6,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,22,5,5,5,5,5,5,4,5,5,5,5,6,4,5,10,2,2,3,5,5,17,23,23,33,9,5,5,5,5,5,5,16,5,5,5,5,5,5,4,5,5,5,5,6,4,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,5,5,5,5,6,4,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,5,5,5,5,6,4,13,8,8,8,9,5,5,17,23,23,23,27,5,5,5,5,5,5,14,5,5,5,5,5,5,4,5,5,5,5,6,4,6,5,5,5,5,5,5,5,5,5,5,22,5,5,5,5,5,5,22,5,5,5,5,5,5,4,5,5,5,5,6,4,6,5,5,5,5,5,5,5,5,5,5,37,2,3,5,5,1,2,36,5,5,5,5,5,5,4,5,5,5,5,6,4,6,5,5,5,5,5,5,5,5,5,5,4,5,6,5,5,4,5,6,5,5,5,5,5,5,4,5,5,5,5,6,4,6,5,5,5,5,5,5,5,5,5,5,4,13,9,5,5,7,12,10,2,2,2,2,2,2,11,5,5,5,5,6,4,10,2,2,2,2,2,2,2,2,2,2,11,6,5,5,5,5,4,5,5,5,5,5,5,5,5,5,5,5,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8,9,5,5,5,5,7,8,8,8,8,8,8,8,8,8,8,8,8,9;";
	roomObject.enemyData.push_back(Vector3f(7, 13, 3)); //Room 1
	roomObject.enemyData.push_back(Vector3f(7, 19, 3));
	roomObject.enemyData.push_back(Vector3f(5, 28, 2)); //Room 2
	roomObject.enemyData.push_back(Vector3f(8, 28, 2));
	roomObject.enemyData.push_back(Vector3f(23, 23, 2)); //Room 3
	roomObject.enemyData.push_back(Vector3f(23, 28, 2));
	roomObject.enemyData.push_back(Vector3f(23, 16, 2)); //Room 4
	roomObject.enemyData.push_back(Vector3f(16, 16, 2)); //Room 5
	roomStringObjects.push_back(roomObject);

	roomObject.roomString = "1,40,23,23,23,23,23,23,39,2,2,2,2,3,5,5,5,5,1,2,2,2,2,40,23,23,23,23,23,23,39,3,4,6,5,5,5,5,5,5,4,5,5,5,5,6,5,5,5,5,4,5,5,5,5,6,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,4,5,5,5,5,10,3,5,5,1,11,5,5,5,5,6,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,4,5,5,5,5,5,6,5,5,4,5,5,5,5,5,6,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,4,5,13,8,8,8,9,5,5,7,8,8,8,12,5,6,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,4,5,6,5,5,5,5,5,5,5,5,5,5,4,5,6,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,7,8,9,5,5,5,5,5,5,5,5,5,5,7,8,9,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,1,3,5,5,5,5,5,5,5,5,5,5,5,5,1,3,5,5,5,5,5,5,4,6,4,6,5,5,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,4,6,4,20,23,15,5,5,17,23,18,6,5,5,5,5,5,5,5,5,5,5,5,5,4,20,23,15,5,5,17,23,18,6,4,6,5,5,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,4,6,7,9,5,5,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,7,9,5,5,5,5,5,5,5,5,7,9,5,5,5,5,5,5,5,5,5,5,5,5,7,9,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1,3,5,5,5,5,5,5,5,5,5,5,5,5,1,3,5,5,5,5,5,5,5,5,1,3,5,5,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,1,3,4,6,5,5,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,5,5,4,6,38,34,23,15,5,5,17,23,33,34,23,23,39,3,5,5,5,5,1,40,23,23,33,34,23,15,5,5,17,23,33,35,22,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,4,6,5,5,5,5,4,6,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,38,9,5,5,5,5,7,35,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,16,5,5,5,5,5,5,16,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,14,5,5,5,5,5,5,14,5,5,5,5,5,5,5,5,5,5,5,22,22,5,5,5,5,5,5,5,5,5,5,5,37,3,5,5,5,5,1,36,5,5,5,5,5,5,5,5,5,5,5,22,25,23,23,23,23,23,23,23,23,23,23,23,33,9,5,5,5,5,7,34,23,23,23,23,23,23,23,23,23,23,23,26;";
	roomObject.enemyData.push_back(Vector3f(7, 24, 3)); //Room 1
	roomObject.enemyData.push_back(Vector3f(25, 24, 3)); //Room 2
	roomObject.enemyData.push_back(Vector3f(5, 16, 2)); //Room 3
	roomObject.enemyData.push_back(Vector3f(16, 16, 2)); //Room 4
	roomObject.enemyData.push_back(Vector3f(16, 8, 3));
	roomObject.enemyData.push_back(Vector3f(27, 16, 2)); //Room 5
	roomObject.enemyData.push_back(Vector3f(5, 5, 2)); //Room 6
	roomObject.enemyData.push_back(Vector3f(27, 5, 2)); //Room 7
	roomStringObjects.push_back(roomObject);


	spriteLoader = _spriteLoader;
	gameRenderer = _renderer;
	tilesetTexture = spriteLoader->GetTexture("tileset.png");

	//Create a vector of tile coordinates
	for (int j = 0; j < 16; j++)
	{
		for (int i = 0; i < 16; i++)
		{

			//create a rectangle that fits the boundaries of the current tile
			IntRect tileBoundary;
			tileBoundary.top = 8 * i;
			tileBoundary.left = 8 * j;
			tileBoundary.width = 8;
			tileBoundary.height = 8;

			//push it to the back of the list
			tilesetCoordinates.push_back(Vector2f(tileBoundary.top, tileBoundary.left));

		}
	}

	//Add 4 doors to the door vector
	for (int i = 0; i <= 3; i++)
	{

		Door door;
		door.Initialise(&tilesetTexture, i, tileScale);
		doors.push_back(door);

	}

	//Push back all room types into the roomtypes vector, setting the enum and weighted chance of creation
	RoomType defaultRoom;
	defaultRoom.weightedChance = 7;
	defaultRoom.roomType = DEFAULT_ROOM;

	RoomType treasureRoom;
	treasureRoom.weightedChance = 3;
	treasureRoom.roomType = TREASURE_ROOM;

	RoomType trapRoom;
	trapRoom.weightedChance = 3;
	trapRoom.roomType = TRAP_ROOM;

	RoomType bossRoom;
	bossRoom.weightedChance = 2;
	bossRoom.roomType = BOSS_ROOM;

	roomTypes.push_back(defaultRoom);
	roomTypes.push_back(treasureRoom);
	roomTypes.push_back(trapRoom);
	roomTypes.push_back(bossRoom);

	//Initialise rendertextures
	mainMapTexture.create(32 * tileScale, 32 * tileScale);
	mapHudTexture.create(207, 207);

	//Load audio
	if (!levelMusic.openFromFile("Game.wav"))
	{
		toolSet.ConsoleOut("Couldn't load music.");
	}
	if (!mainBossMusic.openFromFile("MainBoss.wav"))
	{
		toolSet.ConsoleOut("Couldn't load music.");
	}
	if (!alarm.openFromFile("alarm.wav"))
	{
		toolSet.ConsoleOut("Couldn't load sound.");
	}

	mainBossMusic.setLoop(true);
	levelMusic.setLoop(true);

}

void RoomController::Update()
{

	//Spawn the boss
	if (spawnBoss == true)
	{
		bossSpawnCooldown -= 1;
		if (bossSpawnCooldown > 0)
		{

			bossSpawnCooldown -= 1;
			gameRenderer->ViewSetFollowCoordinates(512, 512);

			if (bossSpawnCooldown <= 0)
			{
				spawnBoss = false;
				enemyController->AddEnemy(512, 512, 0, BOSS_MAINFRAMEGUARD);
				mainBossMusic.play();
			}

		}
	}
	
	//Play alarm at ending sequence
	if (enemyController->endingSequence == true)
	{
		if (playAlarm == true)
		{
			playAlarm = false;
			alarm.play();
		}
	}

	//Draw the minimap to the minimap rendertexture
	mapHudTexture.clear();

	for (int y = 0; y < mapYSize; y++)
	{
		for (int x = 0; x < mapXSize; x++)
		{
			if (map[y][x].hasData == true)
			{

				RectangleShape rect(Vector2f(16, 16));

				int scale = 32;
				int basex = 103 - playerMapCoordinateX * scale;
				int basey = 103 - playerMapCoordinateY * scale;

				rect.setPosition(basex + x * scale - scale/2, basey + y * scale - scale/2);
				rect.setOutlineColor(Color::Black);
				rect.setSize(Vector2f(scale, scale));
				rect.setOutlineThickness(2);

				//Set the colours for different room types
				switch (map[y][x].roomType)
				{
					case START_ROOM:
						rect.setFillColor(Color(0, 228, 54));
						break;
					case TREASURE_ROOM:
						rect.setFillColor(Color(255, 236, 39));
						break;
					case BOSS_ROOM:
						rect.setFillColor(Color(255, 0, 77));
						break;
					case TRAP_ROOM:
						rect.setFillColor(Color(255, 119, 168));
						break;
					default:
						rect.setFillColor(Color(255, 241, 232));
						break;
				}

				//If the room has enemies, make it grey
				if (map[y][x].allEnemiesDefeated == false && map[y][x].roomType != BOSS_ROOM && map[y][x].roomType != TRAP_ROOM)
				{
					rect.setFillColor(Color(194, 195, 199));
				}
				
				//If the player is in the currently drawn room, set it to be blue
				if (playerMapCoordinateY == map[y][x].y && playerMapCoordinateX == map[y][x].x)
				{
					rect.setFillColor(Color::Blue);
				}

				//Draw the base square
				mapHudTexture.draw(rect);

				//Draw the exits
				int rectX = rect.getPosition().x;
				int rectY = rect.getPosition().y;

				RectangleShape exit;

				if (map[y][x].exits[0] == true) //left
				{

					exit.setSize(Vector2f(scale/4, scale/4));
					exit.setFillColor(Color::Black);
					exit.setOrigin(Vector2f(exit.getSize().x / 2, exit.getSize().y / 2));
					exit.setPosition(rectX, rectY + scale / 2 + 1);

					mapHudTexture.draw(exit);
					
				}

				if (map[y][x].exits[1] == true) //right
				{

					exit.setSize(Vector2f(scale / 4, scale / 4));
					exit.setFillColor(Color::Black);
					exit.setOrigin(Vector2f(exit.getSize().x / 2, exit.getSize().y / 2));
					exit.setPosition(rectX + scale, rectY + scale / 2 + 1);

					mapHudTexture.draw(exit);

				}

				if (map[y][x].exits[2] == true) //top
				{

					exit.setSize(Vector2f(scale / 4, scale / 4));
					exit.setFillColor(Color::Black);
					exit.setOrigin(Vector2f(exit.getSize().x / 2, exit.getSize().y / 2));
					exit.setPosition(rectX + scale / 2 + 1, rectY);

					mapHudTexture.draw(exit);

				}

				if (map[y][x].exits[3] == true) //bottom
				{

					exit.setSize(Vector2f(scale / 4, scale / 4));
					exit.setFillColor(Color::Black);
					exit.setOrigin(Vector2f(exit.getSize().x / 2, exit.getSize().y / 2));
					exit.setPosition(rectX + scale / 2 + 1, rectY + scale);

					mapHudTexture.draw(exit);

				}

			}
		}
	}

	//Draw the outline
	RectangleShape outline;
	int thickness = 3;
	int size = 207 - thickness * 2;

	outline.setPosition(thickness, thickness);
	outline.setOutlineThickness(thickness);
	outline.setOutlineColor(Color::White);
	outline.setFillColor(Color::Transparent);
	outline.setSize(Vector2f(size, size));
	mapHudTexture.draw(outline);

	//Actually draw the minimap
	Sprite minimap;
	const Texture& texture = mapHudTexture.getTexture();

	Vector2f viewPos = gameRenderer->GetViewPosition();
	Vector2f viewSize = gameRenderer->GetViewSize();

	int miniMapX = viewPos.x - viewSize.x / 2 + 10;
	int miniMapY = viewPos.y - viewSize.y / 2 + 10;

	minimap.setTexture(texture);
	minimap.setScale(1, -1);
	minimap.setOrigin(0, 208);
	minimap.setPosition(miniMapX, miniMapY);

	gameRenderer->AddGuiDrawable(DEPTH_GUI_MAX, minimap);

	//Draw the map itself
	RenderStates mapState;
	mapState.texture = &tilesetTexture;

	mainMapTexture.clear();
	mainMapTexture.draw(currentRoom, mapState);

	Sprite mainmap;
	const Texture& mapTexture = mainMapTexture.getTexture();

	mainmap.setTexture(mapTexture);
	mainmap.setScale(1, -1);
	mainmap.setOrigin(0, mapTexture.getSize().y);
	mainmap.setPosition(0, 0);

	gameRenderer->AddDrawable(DEPTH_BACKGROUND_MAX, mainmap);

	//Used to update the doors
	for (int i = 0; i < 4; i++)
	{
		doors[i].UpdateFrames();
		gameRenderer->AddDrawable(DEPTH_GAME_MAX, doors[i].doorSprite);
	}

	//Opens doors if all enemies are defeated
	if (numberOfEnemies <= 0)
	{
		map[playerMapCoordinateY][playerMapCoordinateX].allEnemiesDefeated = true;
		roomPlayerIsIn.allEnemiesDefeated = true;
	}
	if (roomPlayerIsIn.allEnemiesDefeated == true)
	{
		roomMoveCooldown -= 1;
	}

	if (roomPlayerIsIn.allEnemiesDefeated == true && roomMoveCooldown <= 0)
	{

		OpenAppropriateDoors();

		//simulate solid doors by shortening the size of the map based on closed or open doors
		if (roomPlayerIsIn.exits[0] == true)MIN_ROOM_POS_X = 32; else MIN_ROOM_POS_X = 64;
		if (roomPlayerIsIn.exits[1] == true)MAX_ROOM_POS_X = 996; else MAX_ROOM_POS_X = 954;
		if (roomPlayerIsIn.exits[2] == true)MIN_ROOM_POS_Y = 32; else MIN_ROOM_POS_Y = 64;
		if (roomPlayerIsIn.exits[3] == true)MAX_ROOM_POS_Y = 996; else MAX_ROOM_POS_Y = 954;

	}

}

void RoomController::NewMap()
{

	toolSet.ConsoleOut("Beginning generation of new map!");

	//Decide on the max number of rooms
	int maxRoomCount = toolSet.RandomRange(15, 30);
	int roomCount = 0;

	//Clear the old map
	map.clear();

	//Then, populate it with blank vectors to get a 2D vector of blank room objects. X and Y coordinates must be swapped to find the appropriate map coordinate, eg map[y][x].
	for (int y = 0; y < mapYSize; y++)
	{
		vector<ChosenRoom> row;

		for (int x= 0; x < mapXSize; x++)
		{
			ChosenRoom chosenRoom;
			chosenRoom.x = x;
			chosenRoom.y = y;
			chosenRoom.hasData = false;
			row.push_back(chosenRoom);
		}

		map.push_back(row);
	}

	//Then, create the starting room at a random position
	ChosenRoom startRoom = map[playerMapCoordinateY][playerMapCoordinateX];

	//Create a room that's devoid of treasure or enemies for the player to start in
	startRoom.allEnemiesDefeated = true;
	startRoom.hasTreasure = false;
	startRoom.roomObject = roomStringObjects[0];
	startRoom.hasData = true;
	startRoom.x = playerMapCoordinateX;
	startRoom.y = playerMapCoordinateY;
	startRoom.roomType = START_ROOM;

	//Set all 4 sides to be true
	startRoom.exits[0] = true;
	startRoom.exits[1] = true;
	startRoom.exits[2] = true;
	startRoom.exits[3] = true;

	//Add this room back into the map
	map[startRoom.y][startRoom.x] = startRoom;

	//Map generation
	vector<ChosenRoom> chosenRooms;
	vector<ChosenRoom> processedRooms;
	chosenRooms.push_back(startRoom);

	//Add a room to the chosen room vector. This vector will be looped, where each entry will be checked for adjacent sides.
	//If the adjacent sides match the given conditions, a room will be placed there, and said room will be pushed back into the list, with the initial room being removed.
	//This will continue until the required number of rooms is reached.

	//Var to decide whether to skip this room
	bool roomSkip = false;
	bool randomFail = false;

	//Generate the map
	while (!chosenRooms.empty() && roomCount != maxRoomCount)
	{

		//Debug divider line
		toolSet.ConsoleOut("----------------------------------------------");

		//First grab the room at the end of the queue and remove it from the back
		ChosenRoom chosenRoom;
		
		if (chosenRooms.size() > 0)
		{
			chosenRoom = chosenRooms[chosenRooms.size() - 1];
			chosenRooms.pop_back();
		}
		else
		{
			chosenRoom = chosenRooms[0];
		}

		//Debug text
		toolSet.ConsoleOut("Beginning iteration of room: x: " + to_string(chosenRoom.x) + " y: " + to_string(chosenRoom.y));

		//Check how many neighbours the room has
		int neighbourCount = 0;

		//Random chance to fail
		if (toolSet.RandomRange(0, 10) > 9 && chosenRoom.roomType != START_ROOM)
		{
			randomFail = true;
			toolSet.ConsoleOut("Room has randomly been picked to quit generation. Skipping...");
		}

		//If we can work on this room...
		if (roomSkip == false && neighbourCount <= 2 && roomCount < maxRoomCount && randomFail == false)
		{

			//Use edge cases to check if the room is up against any side of the map. If so, set that appropriate exit to false
			if (chosenRoom.x == mapXSize - 1) { chosenRoom.exits[1] = false; } //if room is at the right, set the right exit to be false
			if (chosenRoom.x == 0) { chosenRoom.exits[0] = false; } //if the room is at the left, set the left exit to be false
			if (chosenRoom.y == mapYSize - 1) { chosenRoom.exits[3] = false; } //if the room is at the bottom, set the below exit to be false
			if (chosenRoom.y == 0) { chosenRoom.exits[2] = false; } //if the room is at the top, set the above exit to be false


			//This system works through the use of 2 arrays for each room object.
			// 
			//The first is an array of 4 bools | Left Right Top Bottom
			//The second is an array of vectors, the coordinates in these vectors are added to the coordinates of the room.
			//This gives the position of the room for that exit.
			//EG, the coordinates for the left side are -1,0, therefore, it can find the left exit by subtracting exitCoordinate[0].x from the x coordinate of the room on the map.
			//If a bool is set to false in the array, that will line up with the vector.
			for (int i = 0; i < 4; i++)
			{
				bool exit = chosenRoom.exits[i];
				Vector2i exitCoordinate = chosenRoom.exitCoordinates[i];

				if (exit == true)
				{
					if (map[chosenRoom.y + exitCoordinate.y][chosenRoom.x + exitCoordinate.x].hasData == true)
					{
						neighbourCount += 1;
						toolSet.ConsoleOut("Found neighbour room at x: " + to_string(chosenRoom.x + exitCoordinate.x) + " y: " + to_string(chosenRoom.y + exitCoordinate.y) + ".");
					}
					else
					{
						toolSet.ConsoleOut("No neighbour found on this side.");
					}
				}
				else
				{
					toolSet.ConsoleOut("This side does not have an exit.");
				}
			}

			//If the number of neighbours is greater than 2, quit for this iteration
			if (neighbourCount == 2)
			{
				toolSet.ConsoleOut("More than 2 neighbours! Quitting for this iteration.");
				roomSkip = true;
			}


			//Check each side for a neighbour. If a neighbour is found, do not create a room there, otherwise, create a room
			for (int i = 0; i < 4; i++)
			{

				bool exit = chosenRoom.exits[i];
				Vector2i exitCoordinate = chosenRoom.exitCoordinates[i];

				if (exit == true)
				{

					if (map[chosenRoom.y + exitCoordinate.y][chosenRoom.x + exitCoordinate.x].hasData == true)
					{
						toolSet.ConsoleOut("Found neighbour room at x: " + to_string(chosenRoom.x + exitCoordinate.x) + " y: " + to_string(chosenRoom.y + exitCoordinate.y) + ".");
					}
					else
					{

						toolSet.ConsoleOut("Room created at x: " + to_string(chosenRoom.x + exitCoordinate.x) + " y: " + to_string(chosenRoom.y + exitCoordinate.y) + ".");

						ChosenRoom newRoom;
						newRoom.x = chosenRoom.x + exitCoordinate.x;
						newRoom.y = chosenRoom.y + exitCoordinate.y;
						newRoom.roomObject = roomStringObjects[floor(toolSet.RandomRange(0, roomStringObjects.size()))];
						newRoom.hasData = true;

						//Randomly create exits
						for (int i = 0; i < 4; i++)
						{
							newRoom.exits[i] = toolSet.RandomRange(0, 10) > 4;
						}

						//Set exits to false if they line up with the border
						if (newRoom.x == mapXSize - 1) { newRoom.exits[1] = false; } //if room is at the right, set the right exit to be false
						if (newRoom.x == 0) { newRoom.exits[0] = false; } //if the room is at the left, set the left exit to be false
						if (newRoom.y == mapYSize - 1) { newRoom.exits[3] = false; } //if the room is at the bottom, set the below exit to be false
						if (newRoom.y == 0) { newRoom.exits[2] = false; } //if the room is at the top, set the above exit to be false

						//Decide on an edge case basis where to make the entrance to the last room
						if (chosenRoom.x < newRoom.x)newRoom.exits[0] = true;
						if (chosenRoom.x > newRoom.x)newRoom.exits[1] = true;
						if (chosenRoom.y < newRoom.y)newRoom.exits[2] = true;
						if (chosenRoom.y > newRoom.y)newRoom.exits[3] = true;

						//If an adjacent room exists and this room has an exit to it, set the exit of the adjacent room to be true
						if (newRoom.exits[0] == true && map[newRoom.y][newRoom.x - 1].hasData == true)
						{
							map[newRoom.y][newRoom.x - 1].exits[1] = true;
						}
						if (newRoom.exits[1] == true && map[newRoom.y][newRoom.x + 1].hasData == true)
						{
							map[newRoom.y][newRoom.x + 1].exits[0] = true;
						}
						if (newRoom.exits[2] == true && map[newRoom.y - 1][newRoom.x].hasData == true)
						{
							map[newRoom.y - 1][newRoom.x].exits[3] = true;
						}
						if (newRoom.exits[3] == true && map[newRoom.y + 1][newRoom.x].hasData == true)
						{
							map[newRoom.y + 1][newRoom.x].exits[2] = true;
						}

						//Push this room back into the list and the map
						map[newRoom.y][newRoom.x] = newRoom;
						chosenRooms.push_back(newRoom);
						roomCount += 1;
					}

				}

			}

		}

		//Push the current room into the processed rooms vector
		processedRooms.push_back(chosenRoom);
		roomSkip = false;
		randomFail = false;
	}

	//Check if enough rooms have been created
	if (roomCount == maxRoomCount)
	{
		toolSet.ConsoleOut("Maximum rooms reached! Quitting map generation.");
	}

	//Debug divider line
	toolSet.ConsoleOut("----------------------------------------------");
	toolSet.ConsoleOut("Finalising room generation...");
	toolSet.ConsoleOut("Cleaning up loose exits, removing edge exits, and ensuring no room has an exit to an empty cell.");


	//Next, clean up the map data to create a finalised map
	bool bossRoomSet = false;
	for (int my = 0; my < mapYSize; my++)
	{
		for (int mx = 0; mx < mapXSize; mx++)
		{
			//Get the current room
			ChosenRoom room = map[my][mx];
			
			if (room.hasData == true)
			{
				//Do the checks to ensure that all rooms have exits to rooms that have entrances to themselves
				if (room.y != 0 && map[my - 1][mx].exits[3] == true) { room.exits[2] = true; }
				if (room.y != mapYSize - 1 && map[my + 1][mx].exits[2] == true) { room.exits[3] = true; }
				if (room.x != 0 && map[my][mx - 1].exits[1] == true) { room.exits[0] = true; }
				if (room.x != mapXSize - 1 && map[my][mx + 1].exits[0] == true) { room.exits[1] = true; }

				//Do the checks to ensure that no room has an exit to empty space
				if (room.x != 0 && room.exits[0] == true && map[my][mx - 1].hasData == false)room.exits[0] = false;
				if (room.x != mapXSize + 1 && room.exits[1] == true && map[my][mx + 1].hasData == false)room.exits[1] = false;
				if (room.y != 0 && room.exits[2] == true && map[my - 1][mx].hasData == false)room.exits[2] = false;
				if (room.y != mapYSize + 1 && room.exits[3] == true && map[my + 1][mx].hasData == false)room.exits[3] = false;
#
				//Remove edges on the edge of the map
				if (room.x == 0 && room.exits[0] == true)room.exits[0] = false;
				if (room.x == mapXSize - 1 && room.exits[1] == true)room.exits[1] = false;
				if (room.y == 0 && room.exits[2] == true)room.exits[2] = false;
				if (room.y == mapYSize - 1 && room.exits[3] == true)room.exits[3] = false;

				//Set the room type
				//Calculate sum of all weights
				int roomWeightSum = 0;

				for (RoomType roomType : roomTypes)
				{
					roomWeightSum += roomType.weightedChance;
				}

				//Now decide on an int between 1 and roomWeightSum
				int randomRoom = toolSet.RandomRange(1, roomWeightSum);

				for (RoomType roomType : roomTypes)
				{
					if (randomRoom < roomType.weightedChance && room.roomType != START_ROOM && room.roomType != BOSS_ROOM)
					{
						if (roomType.roomType == BOSS_ROOM)
						{
							if (bossRoomSet == false)
							{
								bossRoomSet = true;
								room.roomType = roomType.roomType;
								room.roomObject = roomStringObjects[0];
								map[y][x] = room;
							}
							else
							{
								room.roomType = DEFAULT_ROOM;
							}
						}
						else if(roomType.roomType == TRAP_ROOM)
						{
							numTrapRooms += 1;
							room.roomType = roomType.roomType;
							room.roomObject = roomStringObjects[0];
							map[y][x] = room;
						}
						break;
					}

					randomRoom -= roomType.weightedChance;

				}

				map[room.y][room.x] = room;
			}
		}
	}

	//set the current room object to the finalised start room
	roomPlayerIsIn = map[playerMapCoordinateY][playerMapCoordinateX];

	//Debug Text
	toolSet.ConsoleOut("Finished!");
	toolSet.ConsoleOut(to_string(roomCount) + " rooms have been created.");
}

void RoomController::LoadRoom()
{

	//Grab the room that the player is currently in
	roomPlayerIsIn = map[playerMapCoordinateY][playerMapCoordinateX];

	//Load music
	
	if (enemyController->endingSequence == false)
	{
		if ((roomPlayerIsIn.roomType == TRAP_ROOM || roomPlayerIsIn.roomType == BOSS_ROOM) && roomPlayerIsIn.allEnemiesDefeated == false)
		{
			levelMusic.pause();
		}
		else
		{
			if (levelMusic.getStatus() == SoundSource::Status::Paused || levelMusic.getStatus() == SoundSource::Status::Stopped)
			{
				levelMusic.play();
			}
		}
	}

	//Resize the decompressed room vector
	//Acts as an easy method to grab tile positions, separate from the vector that holds the drawn map
	roomVector.clear();

	for (int y = 0; y < currentRoomXSize; y++)
	{
		
		vector<int> row;

		for (int x = 0; x < currentRoomXSize; x++)
		{

			row.push_back(45);

		}

		roomVector.push_back(row);

	}

	//Close all doors
	for (int i = 0; i <= 3; i++)
	{
		doors[i].SwitchState(1);
	}

	//Grab the room string from that room
	string room = roomPlayerIsIn.roomObject.roomString;
	istringstream roomParser(room);
	string tile;

	//Clear the last room from the vertexarray and resize it to fit the new room
	currentRoom.clear();
	currentRoom.resize(currentRoomXSize * currentRoomYSize * 4);

	//While we still have numbers in the list, go through it int by int until we reach the end
	while (getline(roomParser, tile, ','))
	{

		//Get the current tile
		int currentTile = stoi(tile);

		//Get the position in the tilesheet
		int tileXPos = tilesetCoordinates[currentTile].x;
		int tileYPos = tilesetCoordinates[currentTile].y;

		//Get a pointer to the current tile's quad
		Vertex* quad = &currentRoom[(xIterator + yIterator * currentRoomXSize) * 4];

		//Set the positions of each of the 4 vertices in clockwise order from the top left, accounting for tile scale
		int x1 = tileScale * xIterator;
		int y1 = tileScale * yIterator;
		int x2 = tileScale * (1 + xIterator);
		int y2 = tileScale * (1 + yIterator);
		quad[0].position = Vector2f(x1, y1);
		quad[1].position = Vector2f(x2, y1);
		quad[2].position = Vector2f(x2, y2);
		quad[3].position = Vector2f(x1, y2);

		//Set the texture coordinates of each of the 4 vertices in clockwise order from the top left
		x1 = tileXPos;
		y1 = tileYPos;
		x2 = tileXPos + 8;
		y2 = tileYPos + 8;
		quad[0].texCoords = Vector2f(x1, y1);
		quad[1].texCoords = Vector2f(x2, y1);
		quad[2].texCoords = Vector2f(x2, y2);
		quad[3].texCoords = Vector2f(x1, y2);


		//Place the tile into the decompressed room vector
		roomVector[yIterator][xIterator] = currentTile;


		//Iterate, and move down to the next line if applicable
		if (xIterator < currentRoomXSize - 1)
		{
			xIterator += 1;
		}
		else
		{
			xIterator = 0;
			yIterator += 1;
		}

	}

	//Reset the iterators
	xIterator = 0;
	yIterator = 0;

	//Load all enemies
	numberOfEnemies = 0;
	if (roomPlayerIsIn.allEnemiesDefeated == false)
	{

		if (roomPlayerIsIn.roomType != TRAP_ROOM && roomPlayerIsIn.roomType != BOSS_ROOM)
		{
			for (Vector3f vector : roomPlayerIsIn.roomObject.enemyData)
			{
				numberOfEnemies += 1;
				ENEMY_TYPE enemy = static_cast<ENEMY_TYPE>(rand() % LAST);
				enemyController->AddEnemy(vector.x * 32, vector.y * 32, vector.z * 32, enemy);
			}
		}
		else if(roomPlayerIsIn.roomType == TRAP_ROOM)
		{
			enemyController->AddEnemy(512, 512, 0, BOSS_SHIELD);
			numberOfEnemies += 1;
		}
		else
		{
			spawnBoss = true;
			numberOfEnemies += 1;
		}

	}

}

int RoomController::GetTile(int _x, int _y)
{
	_y /= tileScale;
	_x /= tileScale;

	if (_y > currentRoomYSize)_y = currentRoomYSize;
	if (_y < 0)_y = 0;
	if (_x > currentRoomXSize)_x = currentRoomXSize;
	if (_x < 0)_x = 0;
	
	return roomVector[_y][_x];
}

Vector2i RoomController::RoomTransition(int _x, int _y)
{

	//compare distance of arguments to all doors
	for (Door door : doors)
	{

		if (toolSet.Distance(_x, _y, door.x, door.y) <= 15)
		{

			if (door.IsOpen() && roomMoveCooldown <= 0)
			{

				playerMapCoordinateX += door.xModifier;
				playerMapCoordinateY += door.yModifier;
				LoadRoom();
				roomMoveCooldown = 60;

				return Vector2i(door.xModifier, door.yModifier);

			}

		}

	}

	return Vector2i(0, 0);

}

void RoomController::OpenAppropriateDoors()
{

	//Set the doors to be open or closed
	for (int i = 0; i <= 3; i++)
	{
		//Get the x and y modifier for the current door
		int x = doors[i].xModifier;
		int y = doors[i].yModifier;

		//If the room has an exit on that side and the room it leads to is not a boss room, open the door
		//OR - if the room has an exit on the side and the room it leads to is a boss room AND the number
		//of trap rooms completed is EQUAL to the total number of trap rooms, open the door
		if ((roomPlayerIsIn.exits[i] == true && map[playerMapCoordinateY + y][playerMapCoordinateX + x].roomType != BOSS_ROOM) || (roomPlayerIsIn.exits[i] == true && map[playerMapCoordinateY + y][playerMapCoordinateX + x].roomType == BOSS_ROOM && numTrapRooms == trapRoomsCompleted))
		{
			doors[i].SwitchState(0);
		}
		else
		{
			doors[i].SwitchState(1);
		}
	}
}

bool RoomController::EscapeRoomReached()
{

	if (enemyController->endingSequence == true)
	{
		if (map[playerMapCoordinateY][playerMapCoordinateX].roomType == START_ROOM)
		{

			mainBossMusic.stop();
			alarm.stop();

			return true;

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}


//door methods
void Door::Initialise(Texture* _texture, int _side, int _tileScale)
{

	//Initialise the sprites
	doorSprite.setTexture(*_texture);
	doorSprite.setOrigin(Vector2f(16.5, 5.5));
	doorSprite.setTextureRect(IntRect(0, 24, 34, 12));

	Vector2f scale = doorSprite.getScale();

	doorSprite.setScale(scale.x * _tileScale / 8, scale.y * _tileScale / 8);

	//Set up the transform.
	//Set rotation and position according to the correct place on the map. The doors only have to have their states switched from then on, which switches the sprite texture rectangle.
	//This also means that the player only has to check for the door orientation and state to decide whether they can walk through it or not.
	switch (_side)
	{
		case 0:
			doorSprite.rotate(90);
			doorSprite.setPosition(Vector2f(1 * _tileScale - 16, 16 * _tileScale));
			xModifier = -1;
		break;
		case 1:
			doorSprite.rotate(90);
			doorSprite.setPosition(Vector2f(31 * _tileScale + 16, 16 * _tileScale));
			xModifier = 1;
		break;
		case 2:
			doorSprite.setPosition(Vector2f(16 * _tileScale, 1 * _tileScale - 16));
			yModifier = -1;
		break;
		case 3:
			doorSprite.setPosition(Vector2f(16 * _tileScale, 31 * _tileScale + 16));
			yModifier = 1;
		break;
	}

	x = doorSprite.getPosition().x;
	y = doorSprite.getPosition().y;

}

bool Door::IsOpen()
{
	return !(bool)doorState;
}

void Door::SwitchState(int _state)
{
	doorState = _state;

	switch (doorState)
	{
	case 0:
		doorFrameGoal = 25;
	break;
	case 1:
		doorFrameGoal = 0;
	break;
	}
}

void Door::UpdateFrames()
{

	if (doorFrame < doorFrameGoal-1)
	{
		doorFrame += 1;
	}
	else if(doorFrame > doorFrameGoal+1)
	{
		doorFrame -= 1;
	}

	doorSprite.setTextureRect(IntRect(0, 24 + (12 * doorFrame), 34, 12));

}

void RoomController::Reset()
{

	//Reset room controller
	currentRoom.clear();
	roomVector.clear();
	numberOfEnemies = 0;
	numTrapRooms = 0;
	trapRoomsCompleted = 0;
	playAlarm = false;

	mainBossMusic.stop();
	levelMusic.stop();

}