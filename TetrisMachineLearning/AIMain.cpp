#include "AIMain.h"


AIMain::AIMain(int* score)
{
	_score = score;
	_movementArray[0] = 0;
	_movementArray[1] = 0;
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

void AIMain::GeneratePossibleMoves(Brick* activeBrick)
{
	//iterate through all possible trasnlations in each possible rotation
	if (activeBrick->GetBrickType() != Brick::BrickType::Box)
	{
		for (int rotations = 0; rotations < 4; rotations++)
		{
			_movementArray[1] = rotations;
			while (TetrisHelper::CanBrickMoveLeft(activeBrick, _gameBoard))
			{
				TetrisHelper::MoveBrickLeft(activeBrick, _gameBoard);
				_movementArray[0] -= 1;
			}
			while (TetrisHelper::CanBrickMoveRight(activeBrick, _gameBoard))
			{
				//Check and evaluate position of ghost brick
			}
		}
	}
}

void AIMain::DetermineBestMove(Brick* activeBrick)
{
	//Perform minimax algorithm

	TetrisHelper::PerformMoveSequence(activeBrick, _gameBoard, _score, _movementArray);
}

