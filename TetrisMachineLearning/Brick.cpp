#include "Brick.h"
#include "Errors.h"

Brick::Brick()
{

}

Brick::Brick(BrickType type, Texture* tex)
{
	isLocked = false;
	texture = tex;
	brickType = type;

	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		switch (type)
		{
		case Brick::Box:
			minos[i].setColor(Color(254, 203, 0));
			break;

		case Brick::Line:
			minos[i].setColor(Color(0, 159, 218));
			break;

		case Brick::T:
			minos[i].setColor(Color(149, 45, 152));
			break;

		case Brick::L:
			minos[i].setColor(Color(255, 121, 0));
			break;

		case Brick::J:
			minos[i].setColor(Color(0, 101, 189));
			break;

		case Brick::S:
			minos[i].setColor(Color(105, 190, 40));
			break;

		case Brick::Z:
			minos[i].setColor(Color(237, 41, 57));
			break;

		default:
			fatalError("Error in brick creation! Invalid Brick type: " + brickType);
			break;
		}
		minos[i].setTexture(*texture);
		spriteXBoardPos[i] = 0;
		spriteYBoardPos[i] = 0;
		lastXPos[i] = 0;
		lastYPos[i] = 0;
		markedForDeletion[i] = false;

	}
}

void Brick::LockBrick()
{
	isLocked = true;
}

void Brick::SpriteSetup()
{
	switch (brickType)
	{
	case Brick::Box:
		SetSpriteBoardPos(0, 4, 20);
		SetSpriteBoardPos(1, 4, 21);
		SetSpriteBoardPos(2, 5, 20);
		SetSpriteBoardPos(3, 5, 21);
		break;
	case Brick::Line:
		SetSpriteBoardPos(0, 3, 20);
		SetSpriteBoardPos(1, 4, 20);
		SetSpriteBoardPos(2, 5, 20);
		SetSpriteBoardPos(3, 6, 20);
		break;
	case Brick::T:
		SetSpriteBoardPos(0, 3, 20);
		SetSpriteBoardPos(1, 4, 20);
		SetSpriteBoardPos(2, 4, 21);
		SetSpriteBoardPos(3, 5, 20);
		break;
	case Brick::L:
		SetSpriteBoardPos(0, 3, 20);
		SetSpriteBoardPos(1, 4, 20);
		SetSpriteBoardPos(2, 5, 20);
		SetSpriteBoardPos(3, 5, 21);
		break;
	case Brick::J:
		SetSpriteBoardPos(0, 3, 20);
		SetSpriteBoardPos(1, 3, 21);
		SetSpriteBoardPos(2, 4, 20);
		SetSpriteBoardPos(3, 5, 20);
		break;
	case Brick::S:
		SetSpriteBoardPos(0, 3, 20);
		SetSpriteBoardPos(1, 4, 20);
		SetSpriteBoardPos(2, 4, 21);
		SetSpriteBoardPos(3, 5, 21);
		break;
	case Brick::Z:
		SetSpriteBoardPos(0, 3, 21);
		SetSpriteBoardPos(1, 4, 21);
		SetSpriteBoardPos(2, 4, 20);
		SetSpriteBoardPos(3, 5, 20);
		break;
	default:
		fatalError("Error in sprite setup! Invalid brick type: " + brickType);
		break;
	}
}

bool Brick::IsBrickEmpty()
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (markedForDeletion[i] == false)
		{
			return false;
		}
	}
	return true;
}

void Brick::SetSpriteXPos(int index, int offset)
{
	spriteXBoardPos[index] += offset;

	float xPos = (BOARD_X_OFFSET + spriteXBoardPos[index]) * 36.0f;
	float yPos = 684 - (spriteYBoardPos[index] * 36.0f);
	minos[index].setPosition(xPos, yPos);
}

void Brick::SetSpriteYPos(int index, int offset)
{
	spriteYBoardPos[index] += offset;

	float xPos = (BOARD_X_OFFSET + spriteXBoardPos[index]) * 36.0f;
	float yPos = 684 - (spriteYBoardPos[index] * 36.0f);
	minos[index].setPosition(xPos, yPos);
}

void Brick::SetSpriteBoardPos(int index, int xOffset, int yOffset)
{
	SetSpriteXPos(index, xOffset);
	SetSpriteYPos(index, yOffset);
}

void Brick::RemoveSprite(int index)
{
//	minos[index].setPosition(-36, -36);
	markedForDeletion[index] = true;
}

void Brick::RotateBrick(RotationType type, bool gameBoard[BOARD_WIDTH][BOARD_HEIGHT]) //Take in board pointer, rotate, check for collisions and if found rotate back
{
	DirectionType prevDir = facing;
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		lastXPos[i] = minos[i].getPosition().x;
		lastYPos[i] = minos[i].getPosition().y;
		lastXBoardPos[i] = spriteXBoardPos[i];
		lastYBoardPos[i] = spriteYBoardPos[i];
	}
	if (brickType == Brick::Box)
	{
		return;
	}

	switch (facing)
	{
		if (type == Brick::Clockwise)
		{
#pragma region FacingWest
	case Brick::West:
		facing = Brick::North;

		switch (brickType)
		{
		case Brick::Line:
			SetSpriteBoardPos(0, -1, 2);
			SetSpriteBoardPos(1, 0, 1);
			SetSpriteBoardPos(2, 1, 0);
			SetSpriteBoardPos(3, 2, -1);
			break;

		case Brick::T:
			SetSpriteBoardPos(0, -1, 1);
			SetSpriteBoardPos(2, 1, 1);
			SetSpriteBoardPos(3, 1, -1);
			break;

		case Brick::L:
			SetSpriteBoardPos(0, -1, 1);
			SetSpriteBoardPos(2, 1, -1);
			SetSpriteBoardPos(3, 2, 0);
			break;

		case Brick::J:
			SetSpriteBoardPos(0, -1, 1);
			SetSpriteBoardPos(1, 0, 2);
			SetSpriteBoardPos(3, 1, -1);
			break;

		case Brick::S:
			SetSpriteBoardPos(0, -1, 1);
			SetSpriteBoardPos(2, 1, 1);
			SetSpriteBoardPos(3, 2, 0);
			break;

		case Brick::Z:
			SetSpriteBoardPos(0, 0, 2);
			SetSpriteBoardPos(1, 1, 1);
			SetSpriteBoardPos(3, 1, -1);
			break;
		}
		break;
#pragma endregion
#pragma region FacingNorth
	case Brick::North:
		facing = Brick::East;
		switch (brickType)
		{
		case Brick::Line:
			SetSpriteBoardPos(0, 2, 1);
			SetSpriteBoardPos(1, 1, 0);
			SetSpriteBoardPos(2, 0, -1);
			SetSpriteBoardPos(3, -1, -2);
			break;

		case Brick::T:
			SetSpriteBoardPos(0, 1, 1);
			SetSpriteBoardPos(2, 1, -1);
			SetSpriteBoardPos(3, -1, -1);
			break;

		case Brick::L:
			SetSpriteBoardPos(0, 1, 1);
			SetSpriteBoardPos(2, -1, -1);
			SetSpriteBoardPos(3, 0, -2);
			break;

		case Brick::J:
			SetSpriteBoardPos(0, 1, 1);
			SetSpriteBoardPos(1, 2, 0);
			SetSpriteBoardPos(3, -1, -1);
			break;

		case Brick::S:
			SetSpriteBoardPos(0, 1, 1);
			SetSpriteBoardPos(2, 1, -1);
			SetSpriteBoardPos(3, 0, -2);
			break;

		case Brick::Z:
			SetSpriteBoardPos(0, 2, 0);
			SetSpriteBoardPos(1, 1, -1);
			SetSpriteBoardPos(3, -1, -1);
			break;
		}
		break;
#pragma endregion
#pragma region FacingEast
	case Brick::East:
		facing = Brick::South;
		switch (brickType)
		{
		case Brick::Line:
			SetSpriteBoardPos(0, 1, -2);
			SetSpriteBoardPos(1, 0, -1);
			SetSpriteBoardPos(2, -1, 0);
			SetSpriteBoardPos(3, -2, 1);
			break;

		case Brick::T:
			SetSpriteBoardPos(0, 1, -1);
			SetSpriteBoardPos(2, -1, -1);
			SetSpriteBoardPos(3, -1, 1);
			break;

		case Brick::L:
			SetSpriteBoardPos(0, 1, -1);
			SetSpriteBoardPos(2, -1, 1);
			SetSpriteBoardPos(3, -2, 0);
			break;

		case Brick::J:
			SetSpriteBoardPos(0, 1, -1);
			SetSpriteBoardPos(1, 0, -2);
			SetSpriteBoardPos(3, -1, 1);
			break;

		case Brick::S:
			SetSpriteBoardPos(0, 1, -1);
			SetSpriteBoardPos(2, -1, -1);
			SetSpriteBoardPos(3, -2, 0);
			break;

		case Brick::Z:
			SetSpriteBoardPos(0, 0, -2);
			SetSpriteBoardPos(1, -1, -1);
			SetSpriteBoardPos(3, -1, 1);
			break;
		}
		break;
#pragma endregion
#pragma region FacingSouth
	case Brick::South:
		facing = Brick::West;
		switch (brickType)
		{
		case Brick::Line:
			SetSpriteBoardPos(0, -2, -1);
			SetSpriteBoardPos(1, -1, 0);
			SetSpriteBoardPos(2, 0, 1);
			SetSpriteBoardPos(3, 1, 2);
			break;

		case Brick::T:
			SetSpriteBoardPos(0, -1, -1);
			SetSpriteBoardPos(2, -1, 1);
			SetSpriteBoardPos(3, 1, 1);
			break;

		case Brick::L:
			SetSpriteBoardPos(0, -1, -1);
			SetSpriteBoardPos(2, 1, 1);
			SetSpriteBoardPos(3, 0, 2);
			break;

		case Brick::J:
			SetSpriteBoardPos(0, -1, -1);
			SetSpriteBoardPos(1, -2, 0);
			SetSpriteBoardPos(3, 1, 1);
			break;

		case Brick::S:
			SetSpriteBoardPos(0, -1, -1);
			SetSpriteBoardPos(2, -1, 1);
			SetSpriteBoardPos(3, 0, 2);
			break;

		case Brick::Z:
			SetSpriteBoardPos(0, -2, 0);
			SetSpriteBoardPos(1, -1, 1);
			SetSpriteBoardPos(3, 1, 1);
			break;
#pragma endregion
		}
		break;
		}
	}

	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (gameBoard[spriteXBoardPos[i]][spriteYBoardPos[i]] == true || spriteXBoardPos[i] < 0 || spriteXBoardPos[i] > 9 || spriteYBoardPos[i] < 0)
		{
			UndoRotation();
			facing = prevDir;
			return;
		}
	}
}

void Brick::UndoRotation()
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		minos[i].setPosition(lastXPos[i], lastYPos[i]);
		spriteXBoardPos[i] = lastXBoardPos[i];
		spriteYBoardPos[i] = lastYBoardPos[i];
	}
	std::cout << "Undo Rotation" << std::endl;
}

void Brick::BrickFall()
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		SetSpriteBoardPos(i, 0, -1);
	}
}

bool Brick::operator!=(Brick brick)
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (GetSpriteXPos(i) == brick.GetSpriteXPos(i) &&
			GetSpriteYPos(i) == brick.GetSpriteYPos(i) &&
			brickType == brick.brickType &&
			facing == brick.facing)
		{
			return false;
		}
	}
	return true;
}