#pragma once

#include "Definitions.h"
#include <cmath>
#include <limits>
#include <vector>
class BoardState
{
private:
	bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
	float _evaluatedScore = FLT_MIN;
	int _rotations;
	int _translation;
	int _landingHeight;
	std::vector<BoardState*> _childStates;
public:
	BoardState(bool board[BOARD_WIDTH][BOARD_HEIGHT]);
	BoardState();
	bool(*GetBoard())[BOARD_HEIGHT] { return _gameBoard; }
	float GetScore() { return _evaluatedScore; }
	int GetRotations() { return _rotations; }
	int GetTranslation() { return _translation; }
	int GetLandingHeight() { return _landingHeight; }
	std::vector<BoardState*> GetChildStates() { return _childStates; }
	void SetScore(float val) { _evaluatedScore = val; }
	void AddScore(float val) { _evaluatedScore += val; }
	void SetRotations(int val) { _rotations = val; }
	void SetTranslation(int val) { _translation = val; }
	void SetLandingHeight(int val) { _landingHeight = val; }
	void AddChildState(BoardState* state) { _childStates.emplace_back(state); }
	~BoardState();
};

