#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string_view>

int clamp(int n, int lower, int upper) {
	if (n < lower)
	{
		return lower;
	}
	else if (n > upper)
	{
		return upper;
	}
	else
	{
		return n;
	}
}

class Tetris
{
private:

	// variables

	// if true, is playing the game, if not, is on menu
	bool inGame{ false };

	// OFF GAME:

	// variables for main menu

	const std::string_view options[3]
	{
		"PLAY",
		"SETTINGS",
		"QUIT"
	};
	int currentOption{};

	// IN GAME:

	int score{};

	// pointers for random seed
	int* pTimeSnapshot{};
	int ellapsedTime{};

	// chars used to fill plane
	const char blank{ '.' };
	const char border{ '#' };

	// variables used to make the piece move down at a pace
	const int gravityTimerDefault{ 10 };
	int gravityTimer{ gravityTimerDefault };
	double velocity{ 1 };

	// piece variables
	int pieceCoords[2]{ 4, 0 };
	int currentPiece{ 0 };

	// plane
	const int xLimit{ 12 };
	const int yLimit{ 22 };
	char plane[12][22]{};

	const int uniquePieces[7]{ 1, 2, 3, 7, 11, 15, 17 };
	const char pieces[19][4][4]
	{
		// pieces (mirrored cuz of the weird way coordinates work here dw)
		// thank you gipitee

		// I piece (4 rotations mirrored)
		{
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'},
			{'[','[','[','['},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'}
		},
		// O piece (same in all positions mirrored)
		{
			{'\0','\0','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','\0','\0'}
		},
		// T piece (4 rotations mirrored)
		{
			{'\0','[','\0','\0'},
			{'[','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}

		},
		{
			{'\0','\0','\0','\0'},
			{'[','[','[','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'[','\0','\0','\0'},
			{'[','[','\0','\0'},
			{'[','\0','\0','\0'},
			{'\0','\0','\0','\0'}

		},
		{
			{'\0','[','\0','\0'},
			{'[','[','[','\0'},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		// L piece (4 rotations mirrored)
		{
			{'\0','[','\0','\0'},
			{'\0','[','[','['},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','\0','['},
			{'\0','\0','\0','['},
			{'\0','\0','[','['},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','[','['},
			{'\0','\0','\0','['},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','[','\0'},
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		// J piece (4 rotations mirrored)
		{
			{'\0','\0','\0','['},
			{'\0','[','[','['},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','[','['},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','\0','\0'}
		},
		// S piece (2 rotations mirrored)
		{
			{'\0','\0','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','[','['},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','[','\0'},
			{'\0','[','[','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		// Z piece (2 rotations mirrored)
		{
			{'\0','\0','\0','\0'},
			{'\0','\0','[','['},
			{'\0','[','[','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','\0','\0'}
		}
	};

	const int hitbox[19][2]
	{
		// horizontal hitbox lookup table
		// 
		// numbers meant to be used as in 
		// (pieceCoords[0] = clamp(pieceCoords[0] + 1, HERE, xLimit - HERE);)
		{2, 1},
		{4, 2},
		{3, 1},
		{4, 1},
		{3, 1},
		{4, 1},
		{4, 0},
		{4, 0},
		{4, 1},
		{4, 0},
		{4, 1},
		{4, 0},
		{4, 1},
		{4, 0},
		{4, 1},
		{3, 1},
		{4, 1},
		{3, 1},
		{4, 1}
	};



	// functions

	void newRandomPiece() {
		// take new random piece
		srand((unsigned)(*pTimeSnapshot * ellapsedTime));
		currentPiece = uniquePieces[rand() % 8];
	}

	bool pieceCheck(int x, int y)
	{
		if ((pieceCoords[0] - 4) < x && x <= pieceCoords[0] &&
			(pieceCoords[1] - 4) < y && y <= pieceCoords[1])
		{
			if (pieces[currentPiece]
				[x - (pieceCoords[0] - 3)]
					[y - (pieceCoords[1] - 3)]
					!= '\0'
					&& y >= 0)
			{
				return true;
			}
		}

		return false;
	}

	void drawBorder()
	{
		// draw borders on plane with #
		for (int x{}; x < xLimit; x++)
		{
			for (int y{}; y < yLimit; y++)
			{
				if (x == 0 || x == xLimit - 1)
				{
					plane[x][y] = border;
				}
				else if (y == yLimit - 1)
				{
					plane[x][y] = border;
				}
				else
				{
					plane[x][y] = blank;
				}

			}
		}
	}

	void movePiece(char direction)
	{
		switch (direction)
		{
		case 'a':
			pieceCoords[0] = clamp(
				pieceCoords[0] - 1,
				hitbox[currentPiece][0],
				xLimit - hitbox[currentPiece][1]);
			break;

		case 'd':
			pieceCoords[0] = clamp(
				pieceCoords[0] + 1,
				hitbox[currentPiece][0],
				xLimit - hitbox[currentPiece][1]);
			break;

		case 's':
			pieceCoords[1]++;
			break;
		}
	}

	void spinPiece(int index)
	{
		// spins piece according to next index

		switch (index)
		{
		case 0:   index = 1;  break;
		case 1:   index = 0;  break;
		case 2:   index = 2;  break;
		case 3:   index = 4;  break;
		case 4:   index = 5;  break;
		case 5:   index = 6;  break;
		case 6:   index = 3;  break;
		case 7:   index = 8;  break;
		case 8:   index = 9;  break;
		case 9:   index = 10; break;
		case 10:  index = 7;  break;
		case 11:  index = 12; break;
		case 12:  index = 13; break;
		case 13:  index = 14; break;
		case 14:  index = 11; break;
		case 15:  index = 16; break;
		case 16:  index = 15; break;
		case 17:  index = 18; break;
		case 18:  index = 17; break;
		}

		currentPiece = index;

		//correct position
		pieceCoords[0] = clamp(
			pieceCoords[0],
			hitbox[currentPiece][0],
			xLimit - hitbox[currentPiece][1]);

	}

	void gravity()
	{
		// handle gravity timer
		// (applying gravity when gravityTimer = 0)
		if (gravityTimer > 0)
		{
			gravityTimer--;
		}
		else
		{
			pieceCoords[1]++;

			// check if overlaping a piece, if so, return and call collision
			for (int y{ yLimit + 1 }; y >= 0; y--)
			{
				for (int x{ 1 }; x < xLimit + 1; x++)
				{
					if (pieceCheck(x, y)) {
						if (plane[x][y] == '[' || y >= yLimit - 1)
						{
							pieceCoords[1]--;

							collision();

							break;
						}
					}
				}
			}

			gravityTimer = (int)(gravityTimerDefault / velocity);
		}
	}

	void glue()
	{
		// glue piece on the plane

		// iterate y bottom up
		for (int y{ yLimit + 1 }; y >= 0; y--)
		{
			for (int x{}; x < xLimit; x++)
			{
				if (pieceCheck(x, y))
				{
					plane[x][y] = '[';
				}
			}
		}
	}

	void dissolveRows()
	{
		// destroy row at y and make pieces fall once
		// repeat until none is left

		// iterate y bottom up
		// for each row, check if all columns are filled
		bool destroyed{ true };
		while (destroyed)
		{
			destroyed = false;

			for (int y{ yLimit + 1 }; y >= 0; y--)
			{
				int consecutiveColumns{};

				for (int x{}; x < xLimit; x++)
				{

					if (plane[x][y] == '[')
					{
						consecutiveColumns++;
					}
				}

				// dissolve row if filled
				if (consecutiveColumns == xLimit - 2)
				{
					// iterate from filled row to above
					for (int y2{ y }; y2 >= 0; y2--)
					{
						for (int x{ 1 }; x < xLimit - 1; x++)
						{
							// bring pieces down
							plane[x][y2] = plane[x][y2 - 1];

							if (y2 == 0) // if first row, fill with blank
							{
								plane[x][y2] = blank;
							}
						}
					}

					destroyed = true;
					score++;
				}
			}
		}

	}

	void resetCoords()
	{
		// reset piece coords
		pieceCoords[0] = xLimit / 2;
		pieceCoords[1] = 0;
	}

	void collision()
	{
		// do all the actions needed when collision is detected

		glue();
		dissolveRows();
		newRandomPiece();
		resetCoords();
		checkLose();
	}

	void checkLose()
	{
		// check to see if any piece reach the top

		for (int x{ 1 }; x < xLimit; x++)
		{
			if (plane[x][0] == '[')
			{
				lose();
			}
		}
	}

	void lose()
	{
		inGame = false;
	}

	void cleanPlane()
	{
		// remove pieces

		for (int x{}; x < xLimit; x++)
		{
			for (int y{}; y < yLimit; y++)
			{
				if (plane[x][y] == '[')
				{
					plane[x][y] = blank;
				}
			}
		}
	}

	void reset()
	{
		newRandomPiece();
		resetCoords();
		cleanPlane();
		score = 0;
	}

	void select()
	{
		// when select button is pressed in menu

		switch (currentOption)
		{
		case 0:
			inGame = true;
			reset();
			break;
		case 2:
			exit(0);
			break;
		}
	}

	void renderGame()
	{
		// render the game

		system("cls");

		for (int y{}; y < yLimit; y++)
		{
			char currentChar{};

			for (int x{}; x < xLimit; x++)
			{
				currentChar = plane[x][y];

				// check for piece
				if ((pieceCoords[0] - 4) < x && x <= pieceCoords[0] &&
					(pieceCoords[1] - 4) < y && y <= pieceCoords[1])
				{
					if (pieces[currentPiece]
						[x - (pieceCoords[0] - 3)]
							[y - (pieceCoords[1] - 3)]
							!= '\0'
							&& y >= 0)
					{
						currentChar = '[';
					}
				}

				// really print
				if (currentChar == '[')
				{
					std::cout << "[]";
				}
				else if (currentChar != '\0')
				{
					std::cout << currentChar << " ";
				}
				else
				{
					std::cout << "  ";
				}
			}
			std::cout << std::endl;
		}
	}

	void renderMenu()
	{
		// render main menu of the game
		// the game screen is static on the left, options on the right

		system("cls");

		int i{}; // keep track of what options were printed

		for (int y{}; y < yLimit; y++)
		{
			char currentChar{};

			for (int x{}; x < xLimit; x++)
			{
				currentChar = plane[x][y];

				// check for piece
				if ((pieceCoords[0] - 4) < x && x <= pieceCoords[0] &&
					(pieceCoords[1] - 4) < y && y <= pieceCoords[1])
				{
					if (pieces[currentPiece]
						[x - (pieceCoords[0] - 3)]
							[y - (pieceCoords[1] - 3)]
							!= '\0'
							&& y >= 0)
					{
						currentChar = '[';
					}
				}

				// really print
				if (currentChar == '[')
				{
					std::cout << "[]";
				}
				else if (currentChar != '\0')
				{
					std::cout << currentChar << " ";
				}
				else
				{
					std::cout << "  ";
				}



				// print menu stuff at the right side

				if ((y + 1) % 3 == 0 &&                  // margin
					x == xLimit - 1 &&        // if at the last column
					i < sizeof(options) / sizeof(options[0]))
				{
					char select[3]{ '\0', ' ', ' ' };

					if (i == currentOption)
					{
						select[0] = ' ';
						select[1] = '[';
						select[2] = ']';
					}

					
					std::cout
						<< "    "
						<< select[0]
						<< select[1]
						<< options[i]
						<< select[2];

						i++;
				}

				if (y == 16 && x == xLimit -1)
				{
					std::cout
						<< "     "
						<< "SCORE: " << score;
				}

			}
			std::cout << std::endl;
		}
	}

public:

	void takeInput(char key)
	{
		if (inGame)
		{
			switch (key)
			{
			case 'd':

				movePiece('d');

				// check if overlaping a piece, if so, return and check again.
				while (1)
				{
					bool find{ false };

					for (int y{ yLimit + 1 }; y >= 0 && !find; y--)
					{
						for (int x{}; x <= xLimit + 1; x++)
						{
							if (pieceCheck(x, y)) {
								if (plane[x][y] == '[')
								{
									movePiece('a');

									find = true;
								}
							}
						}
					}

					if (!find)
					{
						break;
					}
				}
				break;

			case 'a':

				movePiece('a');

				// check if overlaping a piece, if so, return and check again.
				while (1)
				{
					bool find{ false };

					for (int y{ yLimit + 1 }; y >= 0 && !find; y--)
					{
						for (int x{}; x <= xLimit + 1; x++)
						{
							if (pieceCheck(x, y)) {
								if (plane[x][y] == '[')
								{
									movePiece('d');

									find = true;
								}
							}
						}
					}

					if (!find)
					{
						break;
					}
				}
				break;

			case 's':
				pieceCoords[1]++;

				// check if overlaping a piece, if so, return and call collision
				for (int y{ yLimit + 1 }; y >= 0; y--)
				{
					for (int x{}; x <= xLimit + 1; x++)
					{
						if (pieceCheck(x, y)) {
							if (plane[x][y] == '[' || y >= yLimit - 1)
							{
								pieceCoords[1]--;

								collision();

								break;
							}
						}
					}
				}

				break;

			case 'w':
				int pastPiece{ currentPiece };

				spinPiece(currentPiece);

				// check if overlaping a piece, if so, return.
				for (int y{ yLimit + 1 }; y >= 0; y--)
				{
					for (int x{}; x <= xLimit + 1; x++)
					{
						if (pieceCheck(x, y)) {
							if (plane[x][y] == '[')
							{
								currentPiece = pastPiece; // revert
								break;
							}
						}
					}
				}

				break;
			}
		}
		else
		{
			switch (key)
			{
				// move up
			case 'w':
				currentOption = clamp(
					currentOption - 1,
					0,
					(sizeof(options) / sizeof(options[0]) - 1));
				break;

				// move down
			case 's':
				currentOption = clamp(
					currentOption + 1,
					0,
					(sizeof(options) / sizeof(options[0]) - 1));
				break;
				// select
			case ' ':
				select();
				break;
			}
		}

	}

	void next(int _ellapsedTime)
	{
		// main event of the game. Call this every frame.

		if (inGame) // play game normally
		{
			ellapsedTime = _ellapsedTime;

			gravity();

			renderGame();
		}
		else // main menu
		{
			ellapsedTime = _ellapsedTime;

			renderMenu();
		}

	}

	// constructor
	Tetris(int* timeSnapshot)
	{
		pTimeSnapshot = timeSnapshot;

		drawBorder();
	}

};

int main()
{
	bool exit{ false };

	// seed for random
	int timeSnapshot{ static_cast<int>(time(0)) };
	int* pTimeSnapshot{ &timeSnapshot };
	int ellapsedTime{};



	// variables for avoiding double click
	const int spinCooldownDefault{ 2 };
	int spinCooldown{};
	const int moveCooldownDefault{ 2 };
	int moveCooldown{};

	Tetris tetris(pTimeSnapshot);

	// main loop
	while (!exit)
	{
		tetris.next(ellapsedTime);

		// subtract cooldowns
		if (spinCooldown > 0)
		{
			spinCooldown--;
		}
		if (moveCooldown > 0)
		{
			moveCooldown--;
		}


		// keys
		if (GetKeyState(VK_LEFT) & 0x8000 && moveCooldown == 0) // left
		{
			tetris.takeInput('a');
			moveCooldown = moveCooldownDefault;
		}
		else if (GetKeyState(VK_RIGHT) & 0x8000 && moveCooldown == 0) // right
		{
			tetris.takeInput('d');
			moveCooldown = moveCooldownDefault;
		}
		if (GetKeyState(VK_DOWN) & 0x8000 && moveCooldown == 0) // down
		{
			tetris.takeInput('s');
		}
		if (GetKeyState(VK_UP) & 0x8000 && spinCooldown == 0) // spin
		{
			tetris.takeInput('w');
			spinCooldown = spinCooldownDefault;
		}
		if (GetKeyState(VK_SPACE) & 0x8000 && moveCooldown == 0) // space
		{
			tetris.takeInput(' ');
		}



		// run at desired (60) fps
		Sleep(17);

		ellapsedTime++;
	}

	return 0;
}