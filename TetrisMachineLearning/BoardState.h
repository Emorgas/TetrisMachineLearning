#pragma once

#include "Definitions.h"
#include <cmath>
#include <limits>
class BoardState
{
private:
	bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
	float _evaluatedScore = FLT_MIN;
	int _rotations;
	int _translation;
public:
	BoardState(bool board[BOARD_WIDTH][BOARD_HEIGHT]);
	bool(*GetBoard())[BOARD_HEIGHT] { return _gameBoard; } //Gotta Check This!!
	float GetScore() { return _evaluatedScore; }
	int GetRotations() { return _rotations; }
	int GetTranslation() { return _translation; }
	void SetScore(float val) { _evaluatedScore = val; }
	void SetRotations(int val) { _rotations = val; }
	void SetTranslation(int val) { _translation = val; }
	~BoardState();
};

