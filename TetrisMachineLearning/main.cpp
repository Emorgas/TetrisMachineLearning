//Lib Includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <queue>

//Self Includes
#include "Definitions.h"
#include "Brick.h"
#include "TetrisHelper.h"
#include "Errors.h"
#include "AIMain.h"
#include "GAMain.h"
using namespace sf;

enum GameState
{
	MainMenu = 1,
	Playing = 2,
	GameOver = 3,
	GameWin = 4
};

//Window Variables
GameState _state;
VideoMode _videoMode;
RenderWindow _window;

//Graphics Variables
Texture _gameBackgroundTexture;
Sprite _backgroundSprite;
Texture _minoTexture;

//Text Variables
Font _gameFont;
Text _scoreText;
Text _nextText;
Text _gameOverText;
Text _brickCountText;
Text _linesClearedText;
Text _levelText;
Text _generationNumberText;
Text _chromosomeNumberText;

//Gameplay Variables
bool _gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
int _score = 0;
int _level = 1;
int _lines = 0;
int _brickCount[NUMBER_OF_BRICK_TYPES] = { 0, 0, 0, 0, 0, 0, 0 };
int _highestLineRemoved = 0;
int _lowestLineRemoved = 24;
int _linesToBeRemoved[4] = { BOARD_HEIGHT - 1, BOARD_HEIGHT - 1, BOARD_HEIGHT - 1, BOARD_HEIGHT - 1 };
int _linesRemoved = 0;
std::vector<Brick*> _brickList;
std::queue<Brick*> _nextQueue;
Clock _lockTimer;
Clock _fallTimer;
Clock _repeatTimer;
bool _checkLockTime = false;
Brick* _activeBrick;

//Genetic Algorithm Variables
int _chromosome = 0;
int _generation = 0;
int _gameNumber = 0;
int _gameScores[GA_PLAYS_PER_CHROMOSME] = { 0, 0, 0 };
GAMain* _GAController;

//AI Variables
AIMain* _AIController;

void InitAndLoad()
{
	srand((int)1234567);

	//Window Variables
	_state = GameState::Playing;
	_videoMode.height = SCREEN_HEIGHT;
	_videoMode.width = SCREEN_WIDTH;
	_window.create(_videoMode, "Tetris - Stephen Moore(11007237)", sf::Style::Close);

	//Graphics
	_gameBackgroundTexture.loadFromFile("Images/background.png");
	_backgroundSprite.setTexture(_gameBackgroundTexture);
	_backgroundSprite.setPosition(0, 0);
	_minoTexture.loadFromFile("Images/mino.png");

	_gameFont.loadFromFile("Fonts/arial.ttf");

	_scoreText.setFont(_gameFont);
	_scoreText.setString("Score: " + std::to_string(_score));
	_scoreText.setCharacterSize(20);
	_scoreText.setColor(Color::Black);
	_scoreText.setPosition(0.0f, 0.0f);

	_linesClearedText.setFont(_gameFont);
	_linesClearedText.setString("Lines Cleared: 0");
	_linesClearedText.setCharacterSize(20);
	_linesClearedText.setColor(Color::Black);
	_linesClearedText.setPosition(0.0f, 25.0f);

	_levelText.setFont(_gameFont);
	_levelText.setString("Level: 1");
	_levelText.setCharacterSize(20);
	_levelText.setColor(Color::Black);
	_levelText.setPosition(0.0f, 50.0f);

	_brickCountText.setFont(_gameFont);
	_brickCountText.setString("Box: 0\nLine: 0\nZ: 0\nS: 0\nL: 0\nJ:0 \nT: 0\n");
	_brickCountText.setCharacterSize(20);
	_brickCountText.setColor(Color::Black);
	_brickCountText.setPosition(0.0f, 75.0f);

	_gameOverText.setFont(_gameFont);
	_gameOverText.setString("GAME OVER! FINAL SCORE: " + std::to_string(_score));
	_gameOverText.setCharacterSize(40);
	_gameOverText.setColor(Color::Red);
	_gameOverText.setPosition(50.0f, 250.0f);

	_nextText.setFont(_gameFont);
	_nextText.setString("Next Piece Type: ");
	_nextText.setCharacterSize(20);
	_nextText.setColor(Color::Black);
	_nextText.setPosition(540.0f, 0.0f);

	_chromosomeNumberText.setFont(_gameFont);
	_chromosomeNumberText.setString("Chromosome #: " + std::to_string(_chromosome));
	_chromosomeNumberText.setCharacterSize(20);
	_chromosomeNumberText.setColor(Color::Black);
	_chromosomeNumberText.setPosition(0.0f, 235.0f);

	_generationNumberText.setFont(_gameFont);
	_generationNumberText.setString("Generation #: " + std::to_string(_generation));
	_generationNumberText.setCharacterSize(20);
	_generationNumberText.setColor(Color::Black);
	_generationNumberText.setPosition(0.0f, 255.0f);

	_AIController = new AIMain(&_score);

	_GAController = new GAMain();
	_GAController->InitialisePopulation();

	_AIController->SetEvaluationModifiers(_GAController->GetChromosome(_chromosome)->alleles);

	TetrisHelper::PopulateBrickQueue(&_nextQueue, &_minoTexture);
	_activeBrick = _nextQueue.front();
	_nextQueue.pop();
	_activeBrick->SpriteSetup();
	_AIController->UpdateGameBoard(_gameBoard);
	_AIController->GeneratePossibleMoves(_activeBrick);

	//Gameplay
	if (_level > 1)
	{
		TetrisHelper::_brickFallTime = pow((0.8f - ((_level - 1) * 0.007)), (_level - 1));
		TetrisHelper::_defaultBrickFallTime = pow((0.8f - ((_level - 1) * 0.007)), (_level - 1));

	}
	else
	{
		TetrisHelper::_brickFallTime = 1.0f;
		TetrisHelper::_defaultBrickFallTime = 1.0f;
	}
}

void ResetGame()
{
	srand((int)1234567);
	//Genetic Algorithm Settings
	_gameScores[_gameNumber] = _score;
	_gameNumber++;
	if (_gameNumber >= GA_PLAYS_PER_CHROMOSME)					 //If we have played enough games with this chromosome
	{
		_gameNumber = 0;
		int temp = 0;
		for (int i = 0; i < GA_PLAYS_PER_CHROMOSME; i++)
		{
			temp += _gameScores[i];					 //Sum and average the scores of each game played
		}
		temp /= GA_PLAYS_PER_CHROMOSME;
		_GAController->SetChromosomeFitness(_chromosome, temp); //Store the average value as the fitness of this chromosome
		std::cout << "Chromosome: " << _chromosome << " Average Score: " << temp << std::endl;

		_chromosome++;											//Move on to the next chromosome
		for (int i = 0; i < GA_PLAYS_PER_CHROMOSME; i++)
		{
			_gameScores[i] = 0;
		}

		if (_chromosome >= GA_POPSIZE)							//if we have played all chromosomes in the population
		{

			_GAController->BeginNewGeneration();				//Call evaluate population which will then proceed to compare all chromosomes
																//And generate children from the parent generation
			_chromosome = 0;
			_generation++;										//Reset the chromosome counter and increment the generation number
		}
		_AIController->SetEvaluationModifiers(_GAController->GetChromosome(_chromosome)->alleles); //Set the Evaluation Modifiers from the new chromosome
	}

	//Genetic Algorithm Settings
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			_gameBoard[x][y] = false;
		}
	}
	_score = 0;
	_level = 1;
	_lines = 0;

	for (int i = 0; i < NUMBER_OF_BRICK_TYPES; i++)
	{
		_brickCount[i] = 0;
	}
	_highestLineRemoved = 0;
	_lowestLineRemoved = 24;
	for (int i = 0; i < 4; i++)
	{
		_linesToBeRemoved[i] = BOARD_HEIGHT - 1;
	}
	_linesRemoved = 0;

	for each (Brick* b in _brickList)
	{
		delete b;
	}

	_brickList.clear();
	std::queue<Brick*> empty;
	std::swap(_nextQueue, empty);
	bool _checkLockTime = false;

	TetrisHelper::PopulateBrickQueue(&_nextQueue, &_minoTexture);
	_activeBrick = _nextQueue.front();
	_nextQueue.pop();
	_activeBrick->SpriteSetup();

	TetrisHelper::_brickFallTime = pow((0.8f - ((_level - 1) * 0.007)), (_level - 1));
	TetrisHelper::_defaultBrickFallTime = pow((0.8f - ((_level - 1) * 0.007)), (_level - 1));

	_scoreText.setString("Score: " + std::to_string(_score));
	_linesClearedText.setString("Lines Cleared: 0");
	_levelText.setString("Level: 1");
	_brickCountText.setString("Box: 0\nLine: 0\nZ: 0\nS: 0\nL: 0\nJ:0 \nT: 0\n");
	_gameOverText.setString("GAME OVER! FINAL SCORE: " + std::to_string(_score));
	_nextText.setString("Next Piece Type: ");
	_generationNumberText.setString("Generation #: " + std::to_string(_generation));
	_chromosomeNumberText.setString("Chromosome #: " + std::to_string(_chromosome));


	_state = GameState::Playing;

	_AIController->UpdateGameBoard(_gameBoard);
	_AIController->GeneratePossibleMoves(_activeBrick);



}

extern void QuitProgram()
{
	_window.close();
}

void HandleEvents()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_window.close();
		}
		else if (event.type == Event::KeyPressed || event.type == Event::KeyReleased)
		{
			TetrisHelper::HandleInput(&event, _activeBrick, _gameBoard, &_score, &_repeatTimer);
		}
	}
}

void LevelUp()
{
	_level++;
	TetrisHelper::_brickFallTime = pow((0.8f - ((_level - 1) * 0.007)), (_level - 1));
	TetrisHelper::_defaultBrickFallTime = pow((0.8f - ((_level - 1) * 0.007)), (_level - 1));
	_levelText.setString("Level: " + std::to_string(_level));

	/*	if (_level == 15)
		{
			_state = GameState::GameWin;
		}*/
}

void LevelUpCheck()
{
	if (_lines != 0 && _lines % 10 == 0)
	{
		LevelUp();
	}
}

void ShiftBoardDown()
{
	int offset = 1;

	for (int y = _lowestLineRemoved + 1; y < BOARD_HEIGHT; y++)
	{
		if (y == _linesToBeRemoved[0] || y == _linesToBeRemoved[1] || y == _linesToBeRemoved[2] || y == _linesToBeRemoved[3])
		{
			offset++;
		}
		else
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				_gameBoard[x][y - offset] = _gameBoard[x][y];
			}
			for (int b = 0; b < _brickList.size(); b++)
			{
				for (int m = 0; m < NUMBER_OF_MINOS_IN_BRICK; m++)
				{
					if (_brickList.at(b)->GetSpriteYPos(m) == y && _brickList.at(b)->GetSpriteDeletionStatus(m) != true)
					{
						_brickList.at(b)->SetSpriteYPos(m, -offset);
					}
				}
			}
		}
	}
}

void CheckForEmptyBricks()
{
	for (int i = 0; i < _brickList.size(); i++)
	{
		if (_brickList.at(i)->IsBrickEmpty())
		{
			delete _brickList.at(i);
			_brickList.erase(_brickList.begin() + i);
		}
	}
}

void AddScore(int manualScore, int linesRemoved)
{
	if (linesRemoved == 1)
	{
		_score += 100 * _level;
		fatalError("Score increased by: " + std::to_string(100 * _level));
	}
	else if (linesRemoved == 2)
	{
		_score += 300 * _level;
		fatalError("Score increased by: " + std::to_string(300 * _level));
	}
	else if (linesRemoved == 3)
	{
		_score += 500 * _level;
		fatalError("Score increased by: " + std::to_string(500 * _level));
	}
	else if (linesRemoved == 4)
	{
		_score += 800 * _level;
		fatalError("Score increased by: " + std::to_string(800 * _level));
	}

	_lines += linesRemoved;
	_linesClearedText.setString("Lines Cleared: " + std::to_string(_lines));
	_score += manualScore;
	if (linesRemoved != 0)
		LevelUpCheck();
}

void RemoveCompletedLines()
{
	_linesRemoved = 0;
	_highestLineRemoved = 0;
	_lowestLineRemoved = 24;
	bool removeLine = false;
	_linesToBeRemoved[0] = 24;
	_linesToBeRemoved[1] = 24;
	_linesToBeRemoved[2] = 24;
	_linesToBeRemoved[3] = 24;

	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		if (TetrisHelper::CheckCompleteLines(_gameBoard, i))
		{
			int tempLineIndex = i;
			for (int x = 0; x < NUMBER_OF_MINOS_IN_BRICK; x++)
			{
				if (tempLineIndex != _linesToBeRemoved[x])
				{
					removeLine = true;
				}
				else
				{
					removeLine = false;
					break;
				}
			}
			if (removeLine == true)
			{
				_linesToBeRemoved[_linesRemoved] = i;
				removeLine = false;
				if (_linesToBeRemoved[_linesRemoved] > _highestLineRemoved)
				{
					_highestLineRemoved = _linesToBeRemoved[_linesRemoved];
				}
				if (_linesToBeRemoved[_linesRemoved] < _lowestLineRemoved)
				{
					_lowestLineRemoved = _linesToBeRemoved[_linesRemoved];
				}
				_linesRemoved++;
			}
		}
	}

	if (_linesRemoved > 0)
	{
		for (int i = 0; i < _brickList.size(); i++)
		{
			Brick* tempBrick = _brickList.at(i);
			for (int s = 0; s < NUMBER_OF_MINOS_IN_BRICK; s++)
			{
				if (tempBrick->GetSpriteYPos(s) == _linesToBeRemoved[0] || tempBrick->GetSpriteYPos(s) == _linesToBeRemoved[1]
					|| tempBrick->GetSpriteYPos(s) == _linesToBeRemoved[2] || tempBrick->GetSpriteYPos(s) == _linesToBeRemoved[3])
				{
					tempBrick->RemoveSprite(s);
				}
			}
		}

		for (int l = 0; l < 4; l++)
		{
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				if (_linesToBeRemoved[l] < BOARD_HEIGHT - 1)
				{
					_gameBoard[i][_linesToBeRemoved[l]] = false;
				}
			}
		}
		AddScore(0, _linesRemoved);
		ShiftBoardDown();
		CheckForEmptyBricks();
	}
}

bool CheckForGameOver()
{
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		if (_activeBrick->GetSpriteYPos(i) < BOARD_SKYLINE)
		{
			return false;
		}
	}
	return true;
}

void Update()
{
	if (_state == GameState::Playing)
	{
		if (!TetrisHelper::CheckFallCollisions(_activeBrick, _gameBoard))
		{
			Time fallTime = _fallTimer.getElapsedTime();
			if (fallTime.asSeconds() >= TetrisHelper::_brickFallTime)
			{
				_activeBrick->BrickFall();
				if (TetrisHelper::_brickFallTime != TetrisHelper::_defaultBrickFallTime)
				{
					AddScore(1, 0);
				}
				_fallTimer.restart();
				_lockTimer.restart();
			}
		}
		else
		{
			if (_checkLockTime == false)
			{
				_checkLockTime = true;
				_lockTimer.restart();
			}
			Time lockTime = _lockTimer.getElapsedTime();
			if (lockTime.asSeconds() >= 0.5f || _activeBrick->IsLocked() == true)
			{
				_activeBrick->LockBrick(); //Brick Locks Down Here!
				_brickCount[_activeBrick->GetBrickType()]++;
				_brickCountText.setString("Box: " + std::to_string(_brickCount[0]) +
					"\nLine: " + std::to_string(_brickCount[1]) +
					"\nZ: " + std::to_string(_brickCount[2]) +
					"\nS: " + std::to_string(_brickCount[3]) +
					"\nL: " + std::to_string(_brickCount[4]) +
					"\nJ: " + std::to_string(_brickCount[5]) +
					"\nT: " + std::to_string(_brickCount[6]) +
					"\n");
				if (CheckForGameOver())
				{
					_state = GameState::GameOver;
				}
				_checkLockTime = false;

				for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
				{
					_gameBoard[_activeBrick->GetSpriteXPos(i)][_activeBrick->GetSpriteYPos(i)] = true;
				}

				_brickList.emplace_back(_activeBrick);

				RemoveCompletedLines();
				//Passing the updated game board to the AIController
				_AIController->UpdateGameBoard(_gameBoard);
				_activeBrick = _nextQueue.front();
				_nextQueue.pop();
				_activeBrick->SpriteSetup();
				if (_nextQueue.empty())
				{
					TetrisHelper::PopulateBrickQueue(&_nextQueue, &_minoTexture);
				}
				_AIController->GeneratePossibleMoves(_activeBrick);
			}
		}
		switch (_nextQueue.front()->GetBrickType())
		{
		case Brick::BrickType::Box:
			_nextText.setString("Next Piece Type: \nBox");
			break;
		case Brick::BrickType::J:
			_nextText.setString("Next Piece Type: \nJ");
			break;
		case Brick::BrickType::L:
			_nextText.setString("Next Piece Type: \nL");
			break;
		case Brick::BrickType::Line:
			_nextText.setString("Next Piece Type: \nLine");
			break;
		case Brick::BrickType::S:
			_nextText.setString("Next Piece Type: \nS");
			break;
		case Brick::BrickType::T:
			_nextText.setString("Next Piece Type: \nT");
			break;
		case Brick::BrickType::Z:
			_nextText.setString("Next Piece Type: \nZ");
			break;
		default:
			_nextText.setString("Next Piece Type: \nUnknown");
			break;
		}
	}

	if (_state == GameState::GameOver)
	{
		//Output game scores here

		ResetGame();
	}
}

void Draw()
{
	_window.clear();
	_window.draw(_backgroundSprite);
	for (int i = 0; i < NUMBER_OF_MINOS_IN_BRICK; i++)
	{
		_window.draw(_activeBrick->GetSpriteArray()[i]);
	}

	for (int i = 0; i < _brickList.size(); i++)
	{
		for (int m = 0; m < NUMBER_OF_MINOS_IN_BRICK; m++)
		{
			if (_brickList.at(i)->GetSpriteDeletionStatus(m) == false)
			{
				_window.draw(_brickList.at(i)->GetSpriteArray()[m]);
			}
		}
	}

	/*for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (_gameBoard[x][y] == true)
			{
				CircleShape c;
				c.setPosition((x + BOARD_X_OFFSET) * 36.0f, 684 - (y * 36.0f));
				c.setRadius(12.0);
				c.setFillColor(Color::Red);
				_window.draw(c);
			}
		}
	}*/
	_generationNumberText.setString("Generation #: " + std::to_string(_generation));
	_chromosomeNumberText.setString("Chromosome #: " + std::to_string(_chromosome));
	_scoreText.setString("Score: " + std::to_string(_score));
	_window.draw(_scoreText);
	_window.draw(_nextText);
	_window.draw(_brickCountText);
	_window.draw(_linesClearedText);
	_window.draw(_levelText);
	_window.draw(_chromosomeNumberText);
	_window.draw(_generationNumberText);

	if (_state == GameState::GameOver)
	{
		_gameOverText.setString("Game Over! Final Score: " + std::to_string(_score));
		_window.draw(_gameOverText);
	}

	_window.display();
}

int main()
{

	InitAndLoad();

	//Game Loop
	while (_window.isOpen())
	{
		_fallTimer.getElapsedTime();
		HandleEvents();
		Update();
		Draw();
	}

	return 0;
}