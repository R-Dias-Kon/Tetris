#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string_view>
#include <mmsystem.h> // sound

#pragma comment(lib, "winmm.lib") // sound



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

	// variables for settings menu

	const std::string_view settingsOptions[5]
	{
		"X SIZE",
		"Y SIZE",
		"VELOCITY",
		"ACCELERATION",
		"RETURN"
	};


	bool inSettings{ false };
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

	int velocity{ 1 };

	int accelerationFactor{ 10 }; // use to divide the default
	const int accelerationTimerDefault{ 5000 };   // 4000 pace to make velocity bigger
	int accelerationTimer{ };
	

	// piece variables
	int pieceCoords[2]{ 4, 0 };
	int currentPiece[2]{ 1, 0 };

	// plane
	const int maxXLimit{ 40 };
	const int maxYLimit{ 40 };
	int xLimit{ 12 };
	int yLimit{ 22 };
	char plane[40][40]{};

	// art
	char art[40][40] = {
	{'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '.', '[', '[', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '[', '[', '.', '.', '[', '[', '.', '.', '.', '#'},
	{'#', '[', '.', '[', '.', '[', '.', '.', '[', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '[', '[', '[', '[', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '[', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '.', '[', '[', '.', '.', '.', '#'},
	{'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '.', '[', '[', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '[', '[', '.', '.', '.', '.', '.', '.', '.', '#'},
	{'#', '[', '.', '[', '.', '[', '.', '[', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '[', '[', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '[', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '[', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '.', '[', '[', '.', '.', '.', '#'},
	{'#', '.', '.', '.', '.', '[', '.', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '[', '[', '[', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '.', '.', '[', '.', '.', '.', '#'},
	{'#', '.', '.', '[', '.', '[', '[', '.', '.', '.', '.', '#'},
	{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
	};


	const int uniquePieces[7]{ 1, 2, 3, 9, 11, 15, 17 };
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
			{'\0','\0','[','\0'},
			{'\0','\0','[','['},
			{'\0','\0','[','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','[','\0'},
			{'\0','[','[','['},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','[','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','\0','\0'},
			{'\0','[','[','['},
			{'\0','\0','[','\0'},
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
			{'\0','\0','[','\0'},
			{'\0','[','[','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','[','['},
			{'\0','\0','\0','\0'}
		},
		// Z piece (2 rotations mirrored)
		{
			{'\0','[','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','\0','\0'},
			{'\0','\0','[','['},
			{'\0','[','[','\0'},
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
		{4, 0},
		{4, 1},
		{3, 1},
		{4, 0},
		{4, 1},
		{4, 0},
		{4, 1},
		{4, 0},
		{4, 1},
		{4, 0},
		{4, 1},
		{4, 1},
		{3, 1},
		{4, 1},
		{3, 1}
	};



	// methods

	void newRandomPiece() {
		// take new random piece, if current and future pieces match, redo.
		currentPiece[0] = currentPiece[1];

		int offset{};

		while (1)
		{
			srand((unsigned)(*pTimeSnapshot * (ellapsedTime + offset)));
			currentPiece[1] = uniquePieces[rand() % 7];

			if (currentPiece[0] != currentPiece[1])
			{
				break;
			}
			else
			{
				offset++;
			}
		}
	}

	bool pieceCheck(int x, int y)
	{
		if ((pieceCoords[0] - 4) < x && x <= pieceCoords[0] &&
			(pieceCoords[1] - 4) < y && y <= pieceCoords[1])
		{
			if (pieces[currentPiece[0]]
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
				hitbox[currentPiece[0]][0],
				xLimit - hitbox[currentPiece[0]][1]);
			break;

		case 'd':
			pieceCoords[0] = clamp(
				pieceCoords[0] + 1,
				hitbox[currentPiece[0]][0],
				xLimit - hitbox[currentPiece[0]][1]);
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

		currentPiece[0] = index;

		//correct position
		pieceCoords[0] = clamp(
			pieceCoords[0],
			hitbox[currentPiece[0]][0],
			xLimit - hitbox[currentPiece[0]][1]);

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

			gravityTimer = gravityTimerDefault / velocity;
		}
	}

	void acceleration()
	{
		// handle acceleration timer
		// (increase velocity when accelerationTimer = 0)

		if (accelerationTimer > 0)
		{
			accelerationTimer--;
		}
		else
		{
			PlaySound(TEXT("accelerate.wav"), NULL, SND_FILENAME | SND_ASYNC);
			velocity++;
			accelerationTimer = accelerationTimerDefault / accelerationFactor;
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
					PlaySound(TEXT("hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}

	}

	void resetCoords()
	{
		// reset piece coords
		pieceCoords[0] = clamp(xLimit / 2, 4, xLimit);
		pieceCoords[1] = 0;
	}

	void collision()
	{
		// do all the actions needed when collision is detected

		PlaySound(TEXT("collide.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
		PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
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

		accelerationTimer = accelerationTimerDefault / accelerationFactor;
		velocity = 1;
		score = 0;
	}

	void select()
	{
		// when select button is pressed in menu

		if (!inSettings)
		{
			switch (currentOption)
			{
			case 0: // START
				inGame = true;
				reset();
				break;

			case 1: // SETTINGS
				inSettings = true;
				currentOption = 0;
				break;

			case 2: // EXIT
				exit(0);
				break;
			}
		}
		else
		{
			switch (currentOption)
			{
			case 4: // RETURN
				inSettings = false;
				currentOption = 0;
				break;
			}
		}

		PlaySound(TEXT("hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
					if (pieces[currentPiece[0]]
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

				if (y == 16 && x == xLimit - 1)
				{
					std::cout
						<< "     "
						<< "SCORE: " << score;
				}
			}

			// print future piece on the side

			if (3 < y && y <= 7) // beetween y6 and y10
			{
				std::cout << "        ";

				for (int x{}; x < 4; x++)
				{
					currentChar = pieces[currentPiece[1]][x][y - 3];
					if (currentChar == '[')
					{
						std::cout << currentChar << ']';
					}
					else
					{
						std::cout << "  ";
					}
				}
			}

			std::cout << "\n";
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
					if (pieces[currentPiece[0]]
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

				if (!inSettings)
				{
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
				}

				else
				{
					if ((y + 1) % 3 == 0 &&        // margin
						x == xLimit - 1 &&        // if at the last column
						i < sizeof(settingsOptions) / sizeof(settingsOptions[0]))
					{
						int amount{}; // recpective amount for option
						switch (i)
						{
						case 0: // X SIZE
							amount = xLimit;
							break;

						case 1: // Y SIZE
							amount = yLimit;
							break;

						case 2: // VELOCITY
							amount = velocity;
							break;

						case 3: // ACCELERTION
							amount = accelerationFactor;
							break;

						case 4: // RETURN
							amount = 0;
						}

						char select[3]{ '\0', ' ', ' ' }; // dw about ts
						if (i == currentOption) // highlight option
						{
							select[0] = ' ';
							select[1] = '[';
							select[2] = ']';
						}

						// print
						std::cout
							<< "    "
							<< select[0]
							<< select[1]
							<< settingsOptions[i] << " " << amount
							<< select[2];

						i++;
					}
				}

				if (y == 16 && x == xLimit - 1) // print the score
				{
					std::cout
						<< "     "
						<< "SCORE: " << score;
				}

			}
			std::cout << "\n";
		}
	}

	void drawArt()
	{
		for (int x{}; x < 40; x++)
		{
			for (int y{}; y < 40; y++)
			{
				plane[x][y] = art[y][x];
			}
		}
	}


public:

	bool getGame()
	{
		return inGame;
	}

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
				int pastPiece{ currentPiece[0] };

				spinPiece(currentPiece[0]);

				// check if overlaping a piece, if so, return.
				for (int y{ yLimit + 1 }; y >= 0; y--)
				{
					for (int x{}; x <= xLimit + 1; x++)
					{
						if (pieceCheck(x, y)) {
							if (plane[x][y] == '[')
							{
								currentPiece[0] = pastPiece; // revert
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
			if (!inSettings)
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
			else
			{
				switch (key)
				{
					// move up
				case 'w':
					currentOption = clamp(
						currentOption - 1,
						0,
						(sizeof(settingsOptions) / sizeof(settingsOptions[0]) - 1));
					break;

					// move down
				case 's':
					currentOption = clamp(
						currentOption + 1,
						0,
						(sizeof(settingsOptions) / sizeof(settingsOptions[0]) - 1));
					break;



				case 'd': // increase

					switch (currentOption)
					{
					case 0: // X SIZE
						xLimit = clamp(
							xLimit + 1,
							6,
							maxXLimit
						);
						break;

					case 1: // Y SIZE
						yLimit = clamp(
							yLimit + 1,
							6,
							maxYLimit
						);
						break;

					case 2: // VELOCITY
						velocity = clamp(
							velocity + 1,
							1,
							20
						);
						break;

					case 3: // ACCELERATION
						accelerationFactor = clamp(
							accelerationFactor + 1,
							1,
							50
						);
					}

					drawBorder();

					break;

				case 'a': // decrease

					switch (currentOption)
					{
					case 0: // X SIZE
						xLimit = clamp(
							xLimit - 1,
							6,
							maxXLimit
						);
						break;

					case 1: // Y SIZE
						yLimit = clamp(
							yLimit - 1,
							6,
							maxYLimit
						);
						break;

					case 2: // VELOCITY
						velocity = clamp(
							velocity - 1,
							1,
							20
						);
						break;

					case 3: // ACCELERATION
						accelerationFactor = clamp(
							accelerationFactor - 1,
							1,
							50
						);
					}

					drawBorder();

					break;

					// select
				case ' ':
					select();
					break;
				}
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
			acceleration();

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

		drawArt();

		next( 0 );
	}

};

int main()
{
	PlaySound(TEXT("intro.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
		if (tetris.getGame())
		{
			tetris.next(ellapsedTime);
		}
		
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

			if (!tetris.getGame())
			{
				tetris.next(ellapsedTime);
			}
		}
		else if (GetKeyState(VK_RIGHT) & 0x8000 && moveCooldown == 0) // right
		{
			tetris.takeInput('d');
			moveCooldown = moveCooldownDefault;

			if (!tetris.getGame())
			{
				tetris.next(ellapsedTime);
			}
		}
		if (GetKeyState(VK_DOWN) & 0x8000 && moveCooldown == 0) // down
		{
			tetris.takeInput('s');

			if (!tetris.getGame())
			{
				tetris.next(ellapsedTime);
			}
		}
		if (GetKeyState(VK_UP) & 0x8000 && spinCooldown == 0) // spin
		{
			tetris.takeInput('w');
			spinCooldown = spinCooldownDefault;

			if (!tetris.getGame())
			{
				tetris.next(ellapsedTime);
			}

		}
		if (GetKeyState(VK_SPACE) & 0x8000 && moveCooldown == 0) // space
		{
			tetris.takeInput(' ');

			if (!tetris.getGame())
			{
				tetris.next(ellapsedTime);
			}
		}



		// run at desired (60) fps
		Sleep(17);

		ellapsedTime++;
	}

	return 0;
}