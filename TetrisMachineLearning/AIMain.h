#pragma once

#include "Definitions.h"
#include "Brick.h"
#include "TetrisHelper.h"

using namespace sf;

class AIMain
{
private:
	int* _score;
	int _movementArray[2]; //{Translations, Rotations}
	int _bestStateScore = INT_MIN;
	bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
public:
	AIMain(int* score);
	~AIMain();
	
	//To be called every time a piece is locked
	void UpdateGameBoard(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]);

	//Decision Methods
	void GeneratePossibleMoves(Brick* activeBrick);
	void DetermineBestMove(Brick* activeBrick);

	//Movement Methods
};

