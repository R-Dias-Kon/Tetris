#include <iostream>
#include <string>
#include <windows.h>
#include<cstdlib>

// pieces (mirrored cuz of the weird way coordinates work here dw)
// thank you gipitee
int uniquePieces[7]{ 1, 2, 3, 7, 11, 15, 17 };
char pieces[19][4][4]
{
	// UNIQUE PIECES: 1, 2, 3, 7, 11, 15, 17

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

// horizontal hitbox lookup table
	// 
	// numbers meant to be used as in 
	// (pieceCoords[0] = clamp(pieceCoords[0] + 1, HERE, xLimit - HERE);)
const int hitbox[19][2]
{
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

int spinIndex(int index)
{
	// returns the right piece index when you try to spin

	switch (index)
	{
	case 0:   return 1;
	case 1:   return 0;
	case 2:   return 2;
	case 3:   return 4;
	case 4:   return 5;
	case 5:   return 6;
	case 6:   return 3;
	case 7:   return 8;
	case 8:   return 9;
	case 9:   return 10;
	case 10:  return 7;
	case 11:  return 12;
	case 12:  return 13;
	case 13:  return 14;
	case 14:  return 11;
	case 15:  return 16;
	case 16:  return 15;
	case 17:  return 18;
	case 18:  return 17;
	}

	return 0;
}

bool pieceCheck(int currentPiece, int pieceCoords[2], int x, int y)
{
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
			return true;
		}
	}

	return false;
}

int main()
{
	bool exit{ false };

	// chars used to fill plane
	const char blank{ '.' };
	const char border{ '#' };

	// seed for random
	int timeSnapshot{(int)time(0)};
	int ellapsedTime{};

	// variables for avoiding double click
	const int spinCooldownDefault{ 2 };
	int spinCooldown{};
	const int moveCooldownDefault{ 2 };
	int moveCooldown{};

	// variables used to make the piece move down at a pace
	const int gravityTimerDefault{ 10 };
	int gravityTimer{ gravityTimerDefault };
	double velocity{ 1 };

	// piece variables
	int pieceCoords[2]{ 4, 0};
	int currentPiece{ 0 };

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

	// main loop
	while (!exit)
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
			gravityTimer = (int)(gravityTimerDefault / velocity);
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


		// move the piece by player input
		if (GetKeyState('D') & 0x8000 && moveCooldown == 0) // right
		{
			pieceCoords[0] = clamp(pieceCoords[0] + 1, hitbox[currentPiece][0],
				                              xLimit - hitbox[currentPiece][1]);
			moveCooldown = moveCooldownDefault;

			for (int y{ yLimit + 1 }; y >= 0; y--)
			{
				for (int x{}; x <= xLimit + 1; x++)
				{
					if (pieceCheck(currentPiece, pieceCoords, x, y)) {
						if (plane[x][y] == '[')
						{
							pieceCoords[0] = clamp(pieceCoords[0] - 1, hitbox[currentPiece][0],
								xLimit - hitbox[currentPiece][1]);
							moveCooldown = moveCooldownDefault;
						}
					}
				}
			}
		}
		else if (GetKeyState('A') & 0x8000 && moveCooldown == 0) // left
		{
			pieceCoords[0] = clamp(pieceCoords[0] - 1, hitbox[currentPiece][0],
				                              xLimit - hitbox[currentPiece][1]);
			moveCooldown = moveCooldownDefault;

			for (int y{ yLimit + 1 }; y >= 0; y--)
			{
				for (int x{}; x <= xLimit + 1; x++)
				{
					if (pieceCheck(currentPiece, pieceCoords, x, y)) {
						if (plane[x][y] == '[')
						{
							pieceCoords[0] = clamp(pieceCoords[0] + 1, hitbox[currentPiece][0],
								xLimit - hitbox[currentPiece][1]);
							moveCooldown = moveCooldownDefault;
						}
					}
				}
			}

		}
		if (GetKeyState('S') & 0x8000) // fall
		{
			pieceCoords[1]++;
		}
		if (GetKeyState('W') & 0x8000 && spinCooldown == 0) // spin
		{
			currentPiece = spinIndex(currentPiece);
			spinCooldown = spinCooldownDefault;

			//correct position
			pieceCoords[0] = clamp(pieceCoords[0], hitbox[currentPiece][0],
				                          xLimit - hitbox[currentPiece][1]);
		}

		// stop piece if collision
		// this is happening when the pieces are overlapping already
		bool glue{ false };
		for (int y{ yLimit + 1 }; y >= 0; y--)
		{
			for (int x{}; x <= xLimit + 1; x++)
			{
				if (pieceCheck(currentPiece, pieceCoords, x, y)) {
					if (y >= yLimit - 1 || plane[x][y] == '[')
					{
						// sinalize to glue piece
						glue = true;
						pieceCoords[1]--;
					}
				}
			}
		}

		// glue piece
		bool fall{ false };
		if (glue) {

			// iterar y de baixo pra cima
			for (int y{ yLimit + 1 }; y >= 0; y--)
			{
				int consecutiveColumns{};

				for (int x{}; x < xLimit; x++)
				{
					if (pieceCheck(currentPiece, pieceCoords, x, y))
					{
						plane[x][y] = '[';
					}


					if (plane[x][y] == '[')
					{
						consecutiveColumns++;
					}
				}
				// destroy filled row
				if (consecutiveColumns == xLimit - 2)
				{
					for (int i{ 1 }; i < xLimit; i++)
					{
						plane[i][y] = blank;
					}
					fall = true;
				}
			}
			// take new random piece
			srand((unsigned)(timeSnapshot* ellapsedTime));
			currentPiece = uniquePieces[rand() % 8];

			pieceCoords[0] = 4;
			pieceCoords[1] = 0;
		}

		// TO DO: FIX THING

		// move everything down
		if (fall)
		{
			bool empty{ true };
			while (empty) 
			{
				// iterate y top to bottom
				// determine first row with a piece
				int yTopPiece{};
				for (int y{}; y < yLimit; y++)
				{
					for (int x{}; x < xLimit; x++) {
						if (plane[x][y] == '[')
						{
							yTopPiece = y;
							break;
						}
					}
				}

				// iterate y bottom up
				// if yDestroy stays at -1, don't engage
				int yDestroy{ -1 };
				for (int y{ yLimit - 1 }; y >= yTopPiece; y--)
				{
					int consecutiveColumns{};

					// check to see if row is empty
					for (int x{ 1 }; x < xLimit; x++)
					{
						if (plane[x][y] == blank)
						{
							consecutiveColumns++;
						}
					}

					if (consecutiveColumns == xLimit - 2)
					{
						yDestroy = y;
						break;
					}
				}

				if (yDestroy > -1)
				{
					// pull everything down one row since yDestroy
					for (int y{ yDestroy }; y > 0; y--)
					{
						for (int x{ 1 }; x < xLimit; x++)
						{
							plane[x][y] = plane[x][y - 1];
						}
					}
				}
				else
				{
					empty = false;
				}
			}
		} 

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

		std::cout << pieceCoords[1] - 3;

		// run at desired (60) fps
		Sleep(17);

		ellapsedTime++;
	}

	return 0;
}