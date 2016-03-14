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
	float _rowsClearedMod, _closedHolesMod, _boardMaxHeightMod, _surfaceRoughnessMod;
	Brick::BrickType _nextPiece;
public:
	AIMain(int* score); //Add setnext piece
	~AIMain();
	
	//To be called every time a piece is locked
	void UpdateGameBoard(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]);

	//Decision Methods
	void GenerateChildren(BoardState* state, Brick* activeBrick);
	void GeneratePossibleMoves(Brick* activeBrick);
	void EvaluateState(BoardState* state);
	int CalculateRowsCleared(BoardState* state);
	int CalculateClosedHoles(BoardState* state);
	int CalculateBoardMaxHeight(BoardState* state);
	int CalculateSurfaceRoughness(BoardState* state);
	void DetermineBestMove(Brick* activeBrick);

	void SetEvaluationModifiers(float values[4]);
};

