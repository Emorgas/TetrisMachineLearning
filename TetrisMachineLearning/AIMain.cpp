#include "AIMain.h"


AIMain::AIMain(int* score)
{
	_score = score;
	_movementArray[0] = 0;
	_movementArray[1] = 0;
	bool tempArray[BOARD_WIDTH][BOARD_HEIGHT] = { false };
	_currentState = new BoardState(tempArray);
}

AIMain::~AIMain()
{
}

void AIMain::UpdateGameBoard(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], Brick nextPiece)
{
	_nextPiece = nextPiece;
	_nextPiece.SpriteSetup();
	for (int h = 0; h < BOARD_HEIGHT; h++)
	{
		for (int w = 0; w < BOARD_WIDTH; w++)
		{
			_gameBoard[w][h] = gameBoard[w][h];
		}
	}
	std::vector<int> test;
	int i = 0;
	test.emplace_back(i);
	std::cout << "Num: " << test.size() << std::endl;
	test.clear();
	std::cout << "Num: " << test.size() << std::endl;

	if (_currentState->GetChildStates().size() > 0)
	{
		_currentState->ClearAllChildStates();
	}
	std::cout << "Number of elements in currentState: " << _currentState->GetChildStates().size() << std::endl;
	//delete _currentState;
	_currentState->Setup(_gameBoard);
	_movementArray[0] = 0;
	_movementArray[1] = 0;
}

void AIMain::GeneratePossibleMoves(Brick* activeBrick, BoardState* state, int searchDepth)
{
	if (searchDepth <= AI_LOOK_AHEAD)
	{
		int maxRots = 4;
		if (activeBrick->GetBrickType() == Brick::BrickType::Box)
		{
			maxRots = 1;
		}
		for (int rot = 0; rot < maxRots; rot++)
		{
			BoardState temp(BoardState(state->GetBoard()));
			temp.SetRotations(rot);
			int trans = 0;
			if (rot > 0)
			{
				activeBrick->RotateBrick(Brick::RotationType::Clockwise, state->GetBoard());
			}
			while (TetrisHelper::CanBrickMoveLeft(activeBrick, state->GetBoard()))
			{
				TetrisHelper::MoveBrickLeft(activeBrick, state->GetBoard()); //At the beginning of a rotation we move the brick to the left
				trans--;
			}
			//Perform drop for leftmost position;
			TetrisHelper::HardDropBrick(activeBrick, temp.GetBoard(), _score, false);
			temp.SetLandingHeight(CalculateLandingHeight(activeBrick));
			temp.SetTranslation(trans);
			for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
			{
				temp.GetBoard()[activeBrick->GetSpriteXPos(i)][activeBrick->GetSpriteYPos(i)] = true;
			}
			state->AddChildState(temp);
			if (searchDepth + 1 <= AI_LOOK_AHEAD)
				GeneratePossibleMoves(&_nextPiece, &state->GetChildStates().at(state->GetChildStates().size() - 1), searchDepth + 1);
			activeBrick->ResetYPos();

			//Begin looping through all positions one step right at a time
			while (TetrisHelper::CanBrickMoveRight(activeBrick, state->GetBoard()))
			{
				BoardState temp2(BoardState(state->GetBoard()));
				//Move the brick one step to the right
				TetrisHelper::MoveBrickRight(activeBrick, state->GetBoard());
				trans++;
				//setup new board state for the graph
				temp2.SetRotations(rot);
				temp2.SetTranslation(trans);
				//Perform 'false' hard drop
				TetrisHelper::HardDropBrick(activeBrick, temp2.GetBoard(), _score, false);
				temp2.SetLandingHeight(CalculateLandingHeight(activeBrick));
				//For each mino in the brick set the position on the board stored in temp
				for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
				{
					temp2.GetBoard()[activeBrick->GetSpriteXPos(i)][activeBrick->GetSpriteYPos(i)] = true;
				}
				//Place temp into the array
				state->AddChildState(temp2);
				if (searchDepth + 1 <= AI_LOOK_AHEAD)
					GeneratePossibleMoves(&_nextPiece, &state->GetChildStates().at(state->GetChildStates().size() - 1), searchDepth + 1);
				//reset the Y position of the active brick
				activeBrick->ResetYPos();
			}
			activeBrick->ResetXPos();
		}
		activeBrick->RotateBrick(Brick::RotationType::Clockwise, state->GetBoard()); //Rotate the brick back to the north facing position
	}
	//std::cout << "Number of possible moves: " << _stateGraph.size() << std::endl;

	//DetermineBestMove(activeBrick);
}

void AIMain::EvaluateState(BoardState* state)
{
	float rowsCleared, closedHoles, boardMaxHeight, pieceLandingHeight, boardMinHeight, surfaceRoughness;

	rowsCleared = CalculateRowsCleared(state);

	closedHoles = CalculateClosedHoles(state);

	pieceLandingHeight = state->GetLandingHeight();

	boardMaxHeight = CalculateBoardMaxAndMinHeight(state, boardMinHeight);

	surfaceRoughness = CalculateSurfaceRoughness(state);

	float score = (rowsCleared * _rowsClearedMod) + (closedHoles * _closedHolesMod) +
		(boardMaxHeight * _boardMaxHeightMod) + (pieceLandingHeight * _landingHeightMod) +
		(surfaceRoughness * _surfaceRoughnessMod);

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


int AIMain::CalculateLandingHeight(Brick* brick)
{
	int lowestSpritePos = 20;
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (brick->GetSpriteYPos(i) < lowestSpritePos)
		{
			lowestSpritePos = brick->GetSpriteYPos(i);
		}
	}
	return lowestSpritePos;
}
void AIMain::CalculateFullCells(BoardState* state, float& heightWeightedCells, float& fullCells)
{
	heightWeightedCells = 0;
	fullCells = 0;
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (state->GetBoard()[x][y] == true)
			{
				fullCells += 1;
				heightWeightedCells += y;
			}
		}
	}
}

int AIMain::CalculateBoardMaxAndMinHeight(BoardState* state, float& fMinHeight)
{
	int columnHeights[BOARD_WIDTH] = { 0 };
	int maxHeight = 0;
	int minHeight = BOARD_HEIGHT;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (state->GetBoard()[x][y] == true)
			{
				columnHeights[x] = y + 1;
			}
		}
	}

	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		if (columnHeights[i] > maxHeight)
		{
			maxHeight = columnHeights[i];
		}
		if (columnHeights[i] < minHeight)
		{
			minHeight = columnHeights[i];
		}
	}
	fMinHeight = minHeight;
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
	float bestChildScore = -FLT_MAX;

	for each (BoardState s in _currentState->GetChildStates())
	{
		index++;
		EvaluateState(&s);
		bestChildScore = -FLT_MAX;
		if (s.GetChildStates().size() > 0)
		{
			for each (BoardState cs in s.GetChildStates())
			{
				EvaluateState(&cs);
				if (cs.GetScore() > bestChildScore)
				{
					bestChildScore = cs.GetScore();
				}
			}
		}
		if (bestChildScore != -FLT_MAX)
		{
			s.AddScore(bestChildScore);
		}

		if (s.GetScore() > bestMoveScore)
		{
			bestMoveScore = s.GetScore();
			bestMoveIndex = index;
		}
	}

	_movementArray[0] = _currentState->GetChildStates().at(bestMoveIndex).GetRotations();
	_movementArray[1] = _currentState->GetChildStates().at(bestMoveIndex).GetTranslation();

	TetrisHelper::PerformMoveSequence(activeBrick, _gameBoard, _score, _movementArray);
}

void AIMain::SetEvaluationModifiers(float values[4])
{
	_rowsClearedMod = values[0];
	_closedHolesMod = values[1];
	_boardMaxHeightMod = values[2];
	_landingHeightMod = values[3];
	_surfaceRoughnessMod = values[4];
}