#include "AIMain.h"


AIMain::AIMain(int* score)
{
	_score = score;
	_movementArray[0] = 0;
	_movementArray[1] = 0;
	_movementArray[2] = 0;
}

AIMain::~AIMain()
{
}

void AIMain::UpdateGameBoard(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT])
{
	for (int h = 0; h < BOARD_HEIGHT; h++)
	{
		for (int w = 0; w < BOARD_WIDTH; w++)
		{
			_gameBoard[w][h] = gameBoard[w][h];
		}
	}
}

void AIMain::DetermineBestMove(Brick* activeBrick)
{
	//Perform minimax algorithm

	TetrisHelper::PerformMoveSequence(activeBrick, _gameBoard, _score, _movementArray);
}

