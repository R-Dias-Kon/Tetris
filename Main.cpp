#include <iostream>
#include <string>
#include <windows.h>

int main()
{

	bool exit{ false };

	// variables used to make the piece move down at a pace
	const int gravityTimerDefault{ 10 };
	int gravityTimer{ gravityTimerDefault };
	double velocity{ 1 };


	// pieces
	const char pieces[23][4][4]
	{
		// I piece (2 rotations)
		{
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'},
			{'[','[','[','['},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','\0','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','[','\0'}
		},
		// O piece (1 rotation, same in all positions)
		{
			{'\0','\0','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','\0','\0'}
		},
		// T piece (4 rotations)
		{
			{'\0','\0','\0','\0'},
			{'[','[','[','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','\0','\0'},
			{'[','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','\0','\0'},
			{'[','[','[','\0'},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'[','\0','\0','\0'},
			{'[','[','\0','\0'},
			{'[','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		// L piece (4 rotations)
		{
			{'\0','\0','[','\0'},
			{'[','[','[','\0'},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'[','\0','\0','\0'},
			{'[','\0','\0','\0'},
			{'[','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'[','[','[','\0'},
			{'[','\0','\0','\0'},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'[','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		// J piece (4 rotations)
		{
			{'[','\0','\0','\0'},
			{'[','[','[','\0'},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','[','\0'},
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'[','[','[','\0'},
			{'\0','\0','[','\0'},
			{'\0','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'[','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		// S piece (2 rotations)
		{
			{'\0','\0','\0','\0'},
			{'\0','[','[','\0'},
			{'[','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'[','\0','\0','\0'},
			{'[','[','\0','\0'},
			{'\0','[','\0','\0'},
			{'\0','\0','\0','\0'}
		},
		// Z piece (2 rotations)
		{
			{'\0','\0','\0','\0'},
			{'[','[','\0','\0'},
			{'\0','[','[','\0'},
			{'\0','\0','\0','\0'}
		},
		{
			{'\0','[','\0','\0'},
			{'[','[','\0','\0'},
			{'[','\0','\0','\0'},
			{'\0','\0','\0','\0'}
		}
	};

	int pieceCoords[2]{ 4, 0};
	int currentPiece{ 12 };


	// plane
	const int xLimit{ 12 };
	const int yLimit{ 22 };
	char plane[xLimit][yLimit]{};

	// draw borders on plane with #
	for (int x{}; x < xLimit; x++)
	{
		for (int y{}; y < yLimit; y++)
		{
			if (x == 0 || x == xLimit - 1)
			{
				plane[x][y] = '#';
			}
			else if (y == yLimit - 1)
			{
				plane[x][y] = '#';
			}
			else
			{
				plane[x][y] = '.';
			}

		}
	}

	// main loop
	while (!exit)
	{
		// handle gravity timer
		// (for applying gravity when gravityTimer = 0)
		if (gravityTimer > 0)
		{
			gravityTimer--;
		}
		else
		{
			pieceCoords[1]++;
			gravityTimer = (int)(gravityTimerDefault / velocity);
		}


		// move the piece by player input
		if (GetKeyState('D') & 0x8000)
		{
			pieceCoords[0]++;
		}
		else if (GetKeyState('A') & 0x8000)
		{
			pieceCoords[0]--;
		}
		if (GetKeyState('S') & 0x8000)
		{
			pieceCoords[1]++;
		}


		// print out the game
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

		std::cout << pieceCoords[1] - 3;

		// run at desired (30) fps
		Sleep(33);
	}

	return 0;
}