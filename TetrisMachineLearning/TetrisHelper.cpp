#include "TetrisHelper.h"

bool TetrisHelper::_rotateKeyPressed = false;
bool TetrisHelper::_repeat = false;
bool TetrisHelper::_hardDropPressed = false;
float TetrisHelper::_brickFallTime;
float TetrisHelper::_defaultBrickFallTime;
float TetrisHelper::_autoRepeatTime = 0.3f;


void TetrisHelper::HandleInput(Event* gameEvent, Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], int* score, Clock* gameTime)
{
	Keyboard::Key keyPressed = (Keyboard::Key)gameEvent->key.code;
	Time repeatTime = gameTime->getElapsedTime();
	switch (keyPressed)
	{
	case Keyboard::Key::Left:
		if (gameEvent->type == Event::KeyPressed)
		{
			if (_repeat == false)
			{
				MoveBrickLeft(activeBrick, gameBoard);
				_repeat = true;
			}
			else if (_repeat == true && repeatTime.asSeconds() >= _autoRepeatTime)
			{
				MoveBrickLeft(activeBrick, gameBoard);
			}
		}
		else if (gameEvent->type == Event::KeyReleased)
		{
			_repeat = false;
			gameTime->restart();
		}
		break;
	case Keyboard::Key::Right:
		if (gameEvent->type == Event::KeyPressed)
		{
			if (_repeat == false)
			{
				MoveBrickRight(activeBrick, gameBoard);
				_repeat = true;
			}
			else if (_repeat == true && repeatTime.asSeconds() >= _autoRepeatTime)
			{
				MoveBrickRight(activeBrick, gameBoard);
			}
		}
		else if (gameEvent->type == Event::KeyReleased)
		{
			_repeat = false;
			gameTime->restart();
		}
		break;
	case Keyboard::Key::Up:
		if (gameEvent->type == Event::KeyPressed)
		{
			if (_rotateKeyPressed == false)
			{
				activeBrick->RotateBrick(Brick::RotationType::Clockwise, gameBoard); //Pass in pointer to board
				_rotateKeyPressed = true;
				std::cout << "Rotate Key Pressed" << std::endl;
			}
		}
		else if (gameEvent->type == Event::KeyReleased)
		{
			_rotateKeyPressed = false;
			std::cout << "Rotate Key Released" << std::endl;
		}
		break;
	case Keyboard::Key::Down:
		if (gameEvent->type == Event::KeyPressed)
		{
			float tempTime = _defaultBrickFallTime / 20;
			_brickFallTime = tempTime;
			std::cout << "Fall Key Pressed: " << _brickFallTime << std::endl;
		}
		if (gameEvent->type == Event::KeyReleased)
		{
			_brickFallTime = _defaultBrickFallTime;
		}
		break;
	case Keyboard::Key::Space:
		if (gameEvent->type == Event::KeyPressed && _hardDropPressed == false)
		{
			HardDropBrick(activeBrick, gameBoard, score);
			_hardDropPressed = true;
		}
		else if (gameEvent->type == Event::KeyReleased)
		{
			_hardDropPressed = false;

		}
	}
}

bool TetrisHelper::CheckFallCollisions(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT])
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (activeBrick->GetSpriteYPos(i) - 1 < 0 || gameBoard[activeBrick->GetSpriteXPos(i)][activeBrick->GetSpriteYPos(i) - 1] == true)
		{
			return true;
		}
	}
	return false;
}

bool TetrisHelper::CheckCompleteLines(bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], int lineIndex)
{
	bool line[BOARD_WIDTH] = { false };

	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		line[x] = gameBoard[x][lineIndex];
	}

	if (std::all_of(std::begin(line), std::end(line), [](bool b) {return b == true;}))
	{
		return true;
	}
	return false;
}

void TetrisHelper::PopulateBrickQueue(std::queue<Brick*>* queue, Texture* tex)
{
	std::vector<Brick*> tempQueue;

	for (int i = 0; i < NUMBER_OF_BRICK_TYPES; i++)
	{
		Brick* newBrick = new Brick((Brick::BrickType)i, tex);
		tempQueue.push_back(newBrick);
	}

	while (tempQueue.size() > 0)
	{
		int rng = rand() % tempQueue.size();
		std::vector<Brick*>::iterator iter = tempQueue.begin();
		queue->push(tempQueue.at(rng));

		while (iter != tempQueue.end())
		{
			if (*iter != tempQueue.at(rng))
			{
				iter++;
			}
			else
			{
				break;
			}
		}
		for each (Brick* b in tempQueue)
		{
			//delete b;
		}
		tempQueue.erase(iter);
	}
}

bool TetrisHelper::CanBrickMoveLeft(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT])
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (activeBrick->GetSpriteXPos(i) + -1 < 0 || activeBrick->GetSpriteXPos(i) + -1 > 9 || gameBoard[activeBrick->GetSpriteXPos(i) + -1][activeBrick->GetSpriteYPos(i)] == true)
		{
			return false;
		}
	}
	return true;
}

bool TetrisHelper::CanBrickMoveRight(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT])
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (activeBrick->GetSpriteXPos(i) + 1 < 0 || activeBrick->GetSpriteXPos(i) + 1 > 9 || gameBoard[activeBrick->GetSpriteXPos(i) + 1][activeBrick->GetSpriteYPos(i)] == true)
		{
			return false;
		}
	}
	return true;
}

void TetrisHelper::MoveBrickLeft(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT])
{

	if (CanBrickMoveLeft(activeBrick, gameBoard))
	{
		for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
		{
			activeBrick->SetSpriteBoardPos(i, -1, 0);
		}
	}
}

void TetrisHelper::MoveBrickRight(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT])
{
	if (CanBrickMoveRight(activeBrick, gameBoard))
	{
		for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
		{
			activeBrick->SetSpriteBoardPos(i, 1, 0);
		}
	}
}
//Last parameter allows a hard drop to be simulated without locking the piece; the default value is true so a 'false' hard drop must be specified
void TetrisHelper::HardDropBrick(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], int* score, bool isTrueDrop)
{
	int lineCounter = 0;
	while (CheckFallCollisions(activeBrick, gameBoard) != true)
	{
		activeBrick->BrickFall();
		lineCounter++;
	}
	if (isTrueDrop)
	{
		activeBrick->LockBrick();
		*score += lineCounter * 2;
	}
}

void TetrisHelper::PerformMoveSequence(Brick* activeBrick, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT], int* score, int moveArray[2])
{
	//MoveArray represents the 3 actions available to the AI and the number of times each should be completed, they are stored in the format {rotations, left moves, right moves}

	//Perform rotations
	for (int r = 0; r < moveArray[0]; r++)
	{
		activeBrick->RotateBrick(Brick::RotationType::Clockwise, gameBoard);
	}

	//Perform Movement
	if (moveArray[1] > 0)
	{
		for (int l = 0; l < moveArray[1]; l++)
			MoveBrickRight(activeBrick, gameBoard);
	}
	else if (moveArray[1] < 0)
	{
		for (int r = 0; r > moveArray[1]; r--)
			MoveBrickLeft(activeBrick, gameBoard);
	}

	//Always end movements with a hard drop
	HardDropBrick(activeBrick, gameBoard, score);
}
