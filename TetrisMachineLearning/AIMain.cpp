#include "AIMain.h"


AIMain::AIMain(int* score)
{
	_score = score;
	_movementArray[0] = 0;
	_movementArray[1] = 0;

	_rowsClearedMod = 1000.5f;
	_closedHolesMod = -2.0f;
	_boardMaxHeightMod = -100.5f;
	_surfaceRoughnessMod = -1.0f;
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
	_movementArray[0] = 0;
	_movementArray[1] = 0;
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
		temp->SetRotations(rot);
		int trans = 0;
		while (TetrisHelper::CanBrickMoveLeft(activeBrick, _gameBoard))
		{
			TetrisHelper::MoveBrickLeft(activeBrick, _gameBoard); //At the beginning of a rotation we move the brick to the left
			trans--;
		}
		//Perform drop for leftmost position;
		TetrisHelper::HardDropBrick(activeBrick, temp->GetBoard(), _score, false);
		temp->SetTranslation(trans);
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
			trans++;
			//setup new board state for the graph
			temp = new BoardState(_gameBoard);
			temp->SetRotations(rot);
			temp->SetTranslation(trans);
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

	//std::cout << "Number of possible moves: " << _stateGraph.size() << std::endl;

	DetermineBestMove(activeBrick);
}

void AIMain::EvaluateState(BoardState* state)
{
	float rowsCleared, closedHoles, boardMaxHeight, surfaceRoughness;

	rowsCleared = CalculateRowsCleared(state);

	closedHoles = CalculateClosedHoles(state);

	boardMaxHeight = CalculateBoardMaxHeight(state);

	surfaceRoughness = CalculateSurfaceRoughness(state);

	float score = (rowsCleared * _rowsClearedMod) + (closedHoles * _closedHolesMod) + (boardMaxHeight * _boardMaxHeightMod) + (surfaceRoughness * _surfaceRoughnessMod);

	state->SetScore(score);
}



int AIMain::CalculateRowsCleared(BoardState* state)
{
	int rowsCleared = 0, highestRowCleared = 0, lowestRowCleared = 24, rowsToBeCleared[4];
	bool removeRow = false;
	rowsToBeCleared[0] = 24;
	rowsToBeCleared[1] = 24;
	rowsToBeCleared[2] = 24;
	rowsToBeCleared[3] = 24;

	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		if (TetrisHelper::CheckCompleteLines(state->GetBoard(), i))
		{
			int tempRowIndex = i;
			for (int x = 0; x < NUMBER_OF_MINOS_IN_BRICK; x++)
			{
				if (tempRowIndex != rowsToBeCleared[x])
				{
					removeRow = true;
				}
				else
				{
					removeRow = false;
					break;
				}
			}
			if (removeRow == true)
			{
				rowsToBeCleared[rowsCleared] = i;
				removeRow = false;
				if (rowsToBeCleared[rowsCleared] > highestRowCleared)
				{
					highestRowCleared = rowsToBeCleared[rowsCleared];
				}
				if (rowsToBeCleared[rowsCleared] < lowestRowCleared)
				{
					lowestRowCleared = rowsToBeCleared[rowsCleared];
				}
				rowsCleared++;
			}
		}
	}

	if (rowsCleared > 0)
	{
		for (int r = 0; r < 4; r++)
		{
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				if (rowsToBeCleared[r] < BOARD_HEIGHT - 1)
				{
					state->GetBoard()[i][rowsToBeCleared[r]] = false;
				}
			}
		}

		int offset = 1;
		for (int y = lowestRowCleared + 1; y < BOARD_HEIGHT; y++)
		{
			if (y == rowsToBeCleared[0] || y == rowsToBeCleared[1] || y == rowsToBeCleared[2] || y == rowsToBeCleared[3])
			{
				offset++;
			}
			else
			{
				for (int x = 0; x < BOARD_WIDTH; x++)
				{
					state->GetBoard()[x][y - offset] = state->GetBoard()[x][y];
				}
			}
		}
	}

	return rowsCleared;
}

int AIMain::CalculateClosedHoles(BoardState* state)
{
	int numberOfClosedHoles = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (state->GetBoard()[x][y] == false)
			{
				//Check above for closed squares
				for (int z = y + 1; z < BOARD_HEIGHT; z++)
				{
					if (state->GetBoard()[x][z] == true)
					{
						numberOfClosedHoles++;
						break;
					}
				}
			}
		}
	}

	//std::cout << "Holes: " << numberOfClosedHoles << std::endl;

	return numberOfClosedHoles;
}

int AIMain::CalculateBoardMaxHeight(BoardState* state)
{
	int maxHeight = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			/*y + 1 because programtically the bottom row is 0,
			however it should be considered row 1 for evaluation purposes
			as the height of any placed block must be at least 1*/
			if (state->GetBoard()[x][y] == true && y + 1 > maxHeight) 
			{
				maxHeight = y + 1;
			}
		}
	}

	return maxHeight;
}

int AIMain::CalculateSurfaceRoughness(BoardState* state)
{
	int columnHeight[BOARD_WIDTH];
	int surfaceRoughness = 0;

	for (int h = 0; h < BOARD_WIDTH; h++)
	{
		columnHeight[h] = 0;
	}

	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			if (state->GetBoard()[x][y] != false)
			{
				columnHeight[x] = y + 1; //Same reasoning for y + 1 as calculating board max height
			}
		}
	}

	for (int i = 0; i < BOARD_WIDTH - 1; i++)
	{
		surfaceRoughness += abs(columnHeight[i] - columnHeight[i + 1]);
	}

	return surfaceRoughness;
}

void AIMain::DetermineBestMove(Brick* activeBrick)
{
	int bestMoveIndex = -1;
	float bestMoveScore = -FLT_MAX;
	int index = -1;
	for each (BoardState* s in _stateGraph)
	{
		index++;
		EvaluateState(s);
		if (s->GetScore() > bestMoveScore)
		{
			bestMoveScore = s->GetScore();
			bestMoveIndex = index;
		}
	}

	_movementArray[0] = _stateGraph.at(bestMoveIndex)->GetRotations();
	_movementArray[1] = _stateGraph.at(bestMoveIndex)->GetTranslation();

	TetrisHelper::PerformMoveSequence(activeBrick, _gameBoard, _score, _movementArray);
}

void AIMain::SetEvaluationModifiers(float values[4])
{
	_rowsClearedMod = values[0];
	_closedHolesMod = values[1];
	_boardMaxHeightMod = values[2];
	_surfaceRoughnessMod = values[3];
}