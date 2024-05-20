#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio/Music.hpp"
#include "Renderer.h"
#include "Tools.h"
#include "Enums.h"
#include "GameObject.h"
#include<iostream>

using namespace sf;
using namespace std;

class EnemyController;

//Object holding data on room types
class RoomType
{

	//variables
public:

	int weightedChance;
	ROOM_TYPE roomType;

};

//Holds the room itself, including data on the placements and radius for enemy spawns
class RoomObjects
{

	//variables
public:

	string roomString;
	vector<Vector3f> enemyData;

};

class Door : public GameObject
{

	//methods
public:

	void Initialise(Texture* _texture, int _side, int _tileScale); //Initialises the door with location and rotation paramaters
	void UpdateFrames(); //Updates the door frames depending on if the door is open or closed
	bool IsOpen(); //Returns whether the room is open or closed
	void SwitchState(int _state); //Switches the state of the door to the given state

private:

	//variables
public:

	//door position
	int x;
	int y;
	
	//where to move the player to when changing room by that door
	int xModifier = 0;
	int yModifier = 0;

	Sprite doorSprite; //The door sprite

private:

	int doorState; //Whether the door is open or closed

	//The door variables
	int doorFrame = 0;
	int doorFrameGoal = 0;
};


//These contain the room with context, unlike RoomIndex which is simply raw room data.
//ChosenRoom adds needed info to the map like whether it has enemies, where they are, whether it has treasure, etc. A pointer to the room itself is stored as a variable.
class ChosenRoom
{

public:

	RoomObjects roomObject; //The room tile data as a string
	bool hasTreasure; //Whether the room will have treasure in it or not

	vector<Vector2f> enemyPositions; //Positions of enemies
	bool allEnemiesDefeated = false; //Whether the enemies have been defeated or not, if not, respawn them upon room reentry
	bool hasData = false; //Used to see if this room has actually been set. If not, do not allow entry. Essentially a blank void with no data inside

	//exits
	vector<bool> exits = { false,false,false,false };

	vector<Vector2i> exitCoordinates = {
		Vector2i(-1, 0), //left
		Vector2i(1, 0), //right
		Vector2i(0, -1), //top
		Vector2i(0, 1) //bottom
	};

	//Enemy positions and patrol radius
	vector<Vector3f> enemyData;
	
	//room position
	int x;
	int y;

	//used to decide whether the room is a dead end or not
	bool deadEnd = false;

	//used to decide room type
	ROOM_TYPE roomType = DEFAULT_ROOM;
};

//The main room controller.
class RoomController : public GameObject
{

	//methods
public:

	void Initialise(GameRenderer* _renderer, SpriteLoader* _spriteLoader); //Initialises a room controller
	void NewMap(); //Generates a new map
	void LoadRoom(); //Clears the last room generated on screen and generates a room from the given string
	void DrawMap(RenderTarget& _target, RenderStates _states); //For debugging
	void Update(); //Updates door frames, etc
	Vector2i RoomTransition(int _x, int _y); //Used to transition between rooms
	void OpenAppropriateDoors();
	int GetTile(int _x, int _y); //Grabs a tile from a given coordinate, returns index of tile sprite
	bool EscapeRoomReached(); //Used by the Game class to check if the player has eached the escape room
	void Reset();

private:

	//variables
public:

	int MIN_ROOM_POS_X = 32;
	int MIN_ROOM_POS_Y = 32;
	int MAX_ROOM_POS_X = 991;
	int MAX_ROOM_POS_Y = 991;

	//Load the enemy controller
	EnemyController* enemyController;
	int numberOfEnemies;

	int numTrapRooms = 0; //The number of trap rooms
	int trapRoomsCompleted = 0; //The number of trap rooms completed by the player

	//Boss variables
	bool spawnBoss = false;
	int bossSpawnCooldown = 180;

private:

	Vector2f roomIndex; //The index of the room based on the 2D map

	vector<vector<ChosenRoom>> map; //The map

	int mapXSize = 13; //The x size of the map
	int mapYSize = 13; //The y size of the map

	int currentRoomXSize = 32; //The x size of the current room
	int currentRoomYSize = 32; //The y size of the current room

	int tileScale = 32; //The scale of the tiles

	int xIterator = 0; //The iterator for room loading x
	int yIterator = 0; //The iterator for room loading y

	int playerMapCoordinateX = 7; //The x coordinate on the map
	int playerMapCoordinateY = 7; //The y coordinate on the map

	int roomMoveCooldown = 60; //Cooldown after entering a room to prevent leaving for bug prevention

	VertexArray currentRoom; //The current room, copied from the appropriate place in the map
	ChosenRoom roomPlayerIsIn; //An object containing the current room data, used for doors, enemies, etc
	vector<RoomObjects> roomStringObjects; //An initial list of strings that contain the entire room, to be decompressed into usable room objects
	
	vector<vector<int>> roomVector; //A vector holding the decompressed room string

	vector<Door> doors; //A vector of doors

	vector<RoomType> roomTypes; //A vector holding every possible type of room excluding the start room
	
	//Load the tileset and all images
	GameRenderer* gameRenderer;

	Texture tilesetTexture;
	vector<Sprite> tilesetImages;
	vector<Vector2f> tilesetCoordinates;
	RenderTexture mainMapTexture;
	RenderTexture mapHudTexture;

	//Load music
	Music levelMusic;
	Music miniBossMusic;
	Music mainBossMusic;
	Music alarm;
	bool playAlarm = true;

};