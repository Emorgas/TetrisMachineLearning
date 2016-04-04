#include "BoardState.h"

BoardState::BoardState()
{

}

BoardState::BoardState(bool board[BOARD_WIDTH][BOARD_HEIGHT])
{
	_rotations = 0;
	_translation = 0;
	for (int h = 0; h < BOARD_HEIGHT; h++)
	{
		for (int w = 0; w < BOARD_WIDTH; w++)
		{
			_gameBoard[w][h] = board[w][h];
		}
	}
}

BoardState::~BoardState()
{
	
}

void BoardState::Setup(bool board[BOARD_WIDTH][BOARD_HEIGHT])
{
	_rotations = 0;
	_translation = 0;
	for (int h = 0; h < BOARD_HEIGHT; h++)
	{
		for (int w = 0; w < BOARD_WIDTH; w++)
		{
			_gameBoard[w][h] = board[w][h];
		}
	}
}

void BoardState::ClearAllChildStates()
{
	_childStates.clear();
}
