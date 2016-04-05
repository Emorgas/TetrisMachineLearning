#pragma once

#include "Brick.h"
#include "Definitions.h"

#include <queue>
#include <time.h>

using namespace sf;

class TetrisHelper
{
private:
	static bool _rotateKeyPressed;
	static float _autoRepeatTime;
	static bool _repeat;
	static bool _hardDropPressed;
public:
	static float _brickFallTime;
	static float _defaultBrickFallTime;

	static void HandleInput(Event* gameEvent, Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], unsigned long* score, Clock* gameTime);

	static bool CheckFallCollisions(Brick* activeBrick, bool gameboard[BOARD_WIDTH][BOARD_HEIGHT]);

	static bool CheckCompleteLines(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], int lineIndex);

	static void PopulateBrickQueue(std::queue<Brick*>* queue, Texture* tex);

	//static void MoveBrick(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], int xOffset);

	static bool CanBrickMoveLeft(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]);

	static bool CanBrickMoveRight(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]);

	//Individual Moves
	static void MoveBrickLeft(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]);

	static void MoveBrickRight(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]);

	static void HardDropBrick(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], unsigned long* score, bool isTrueDrop = true);

	//AI Control Method
	static void PerformMoveSequence(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], unsigned long* score, int moveArray[2]);

};

