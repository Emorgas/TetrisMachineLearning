#pragma once

#include "Definitions.h"
#include "Brick.h"
#include "TetrisHelper.h"

using namespace sf;

class AIMain
{
private:
	int* _score;
	int _movementArray[3]; //{ Rotations, Left Moves, Right Moves } left and right cannot both be above 0
	bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
public:
	AIMain(int* score);
	~AIMain();
	
	//To be called every time a piece is locked
	void UpdateGameBoard(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]);

	//Decision Methods
	void DetermineBestMove(Brick* activeBrick);

	//Movement Methods
};

