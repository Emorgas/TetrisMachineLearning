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
	_stateGraph.clear();
}

void AIMain::GeneratePossibleMoves(Brick* activeBrick)
{
	int maxRots = 4;
	if (activeBrick->GetBrickType() == Brick::BrickType::Box)
	{
		maxRots = 1;
	}
	for (int rot = 0; rot < maxRots; rot++)
	{
		BoardState* temp = new BoardState(_gameBoard);
		while (TetrisHelper::CanBrickMoveLeft(activeBrick, _gameBoard))
		{
			TetrisHelper::MoveBrickLeft(activeBrick, _gameBoard); //At the beginning of a rotation we move the brick to the left
			_movementArray[1] -= 1;
		}
		//Perform drop for leftmost position;
		TetrisHelper::HardDropBrick(activeBrick, temp->GetBoard(), _score, false);
		for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
		{
			temp->GetBoard()[activeBrick->GetSpriteXPos(i)][activeBrick->GetSpriteYPos(i)] = true;
		}
		_stateGraph.emplace_back(temp);
		activeBrick->ResetYPos();

		//Begin looping through all positions one step right at a time
		while (TetrisHelper::CanBrickMoveRight(activeBrick, _gameBoard))
		{
			//Move the brick one step to the right
			TetrisHelper::MoveBrickRight(activeBrick, _gameBoard);
			//setup new board state for the graph
			temp = new BoardState(_gameBoard);
			//Perform 'false' hard drop
			TetrisHelper::HardDropBrick(activeBrick, temp->GetBoard(), _score, false);
			//For each mino in the brick set the position on the board stored in temp
			for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
			{
				temp->GetBoard()[activeBrick->GetSpriteXPos(i)][activeBrick->GetSpriteYPos(i)] = true;
			}
			//Place temp into the array
			_stateGraph.emplace_back(temp);
			//reset the Y position of the active brick
			activeBrick->ResetYPos();
		}
		activeBrick->ResetXPos();
	}

	std::cout << "Number of possible moves: " << _stateGraph.size();
}

void AIMain::DetermineBestMove(Brick* activeBrick)
{
	//Perform minimax algorithm

	TetrisHelper::PerformMoveSequence(activeBrick, _gameBoard, _score, _movementArray);
}

