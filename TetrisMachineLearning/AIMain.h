#pragma once

#include "Definitions.h"
#include "BoardState.h"
#include "Brick.h"
#include "TetrisHelper.h"
#include <vector>
using namespace sf;

class AIMain
{
private:
	int* _score;
	int _movementArray[2]; //{Rotations, Translation}
	int _bestStateScore = INT_MIN;
	bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
	std::vector<BoardState*> _stateGraph;
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

