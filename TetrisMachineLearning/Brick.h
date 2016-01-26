#pragma once
//Lib Includes
#include <SFML\Graphics.hpp>
#include <iostream>

//Self Includes
#include "Definitions.h"

using namespace sf;
class Brick
{
public:
	enum BrickType
	{
		Box,
		Line,
		Z,
		S,
		L,
		J,
		T
	};
	enum DirectionType
	{
		North,
		East,
		South,
		West,
		NullDir
	};
	enum RotationType
	{
		Clockwise,
		AntiClockwise
	};
private:
	Sprite minos[NUMBER_OF_MINOS_IN_BRICK];
	float lastXPos[NUMBER_OF_MINOS_IN_BRICK];
	float lastYPos[NUMBER_OF_MINOS_IN_BRICK];
	float lastXBoardPos[NUMBER_OF_MINOS_IN_BRICK];
	float lastYBoardPos[NUMBER_OF_MINOS_IN_BRICK];
	BrickType brickType;
	DirectionType facing;
	int spriteXBoardPos[NUMBER_OF_MINOS_IN_BRICK];
	int spriteYBoardPos[NUMBER_OF_MINOS_IN_BRICK];
	bool isLocked = false;
	bool markedForDeletion[NUMBER_OF_MINOS_IN_BRICK];
	int pivotSprite;
	Texture* texture;
public:
	Brick();
	Brick(BrickType type, Texture* tex);
	void SpriteSetup();

	void LockBrick();
	bool IsLocked() { return isLocked; }
	Sprite* GetSpriteArray() { return minos; }
	int GetSpriteXPos(int index) { return spriteXBoardPos[index]; }
	int GetSpriteYPos(int index) { return spriteYBoardPos[index]; }
	BrickType GetBrickType() { return brickType; }
	DirectionType GetDirectionType() { return facing; }
	bool GetSpriteDeletionStatus(int index) { return markedForDeletion[index]; }
	bool IsBrickEmpty();

	void SetSpriteXPos(int index, int offset);
	void SetSpriteYPos(int index, int offset);
	void SetSpriteBoardPos(int index, int xOffset, int yOffset);

	void BrickFall();

	void RemoveSprite(int index);
	void RotateBrick(RotationType type, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]); //Take in board pointer
	void UndoRotation();
	bool operator!=(Brick brick);

};

