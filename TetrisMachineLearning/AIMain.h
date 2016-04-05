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
	unsigned long* _score;
	int _movementArray[2]; //{Rotations, Translation}
	bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
	BoardState* _currentState;
	float _rowsClearedMod, _closedHolesMod, _boardMaxHeightMod, _landingHeightMod, _surfaceRoughnessMod;
	Brick _nextPiece;
public:
	AIMain(unsigned long* score); //Add setnext piece
	~AIMain();
	
	BoardState* GetCurrentState() { return _currentState; }

	//To be called every time a piece is locked
	void UpdateGameBoard(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], Brick nextPiece);

	//Decision Methods
	void GeneratePossibleMoves(Brick* activeBrick, BoardState* state, int searchDepth);
	void EvaluateState(BoardState* state);
	int CalculateRowsCleared(BoardState* state);
	int CalculateClosedHoles(BoardState* state);
	int CalculateLandingHeight(Brick* brick);
	void CalculateFullCells(BoardState *state, float& heightWeightedCells, float& fullCells);
	int CalculateBoardMaxAndMinHeight(BoardState* state, float& fMinHeight);
	int CalculateSurfaceRoughness(BoardState* state);
	void DetermineBestMove(Brick* activeBrick);

	void SetEvaluationModifiers(float values[4]);
};

