#pragma once

#include "Definitions.h"
#include <cmath>
class BoardState
{
private:
	bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
	int _evaluatedScore = INT_MIN;
	int _rotations;
	int _translation;
public:
	BoardState(bool board[BOARD_WIDTH][BOARD_HEIGHT]);
	bool(*GetBoard())[BOARD_HEIGHT] { return _gameBoard; } //Gotta Check This!!
	int GetScore() { return _evaluatedScore; }
	~BoardState();
};

