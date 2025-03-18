#include <iostream>
#include <string>
#include <windows.h>

int main() {

	bool exit{ false };

	int ytest{};
	int xtest{};

	const int xLimit{ 12 };
	const int yLimit{ 22 };
	char plane[xLimit][yLimit];

	// draw borders on plane with #
	for (int x{}; x < xLimit; x++)
	{
		xtest++;
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
			ytest++;

		}
	}

	std::string direction{ "right" };

	// main loop
	while (!exit)
	{
		// store player input
		if (GetKeyState('W') & 0x8000)
		{
			direction = "up";
		}
		if (GetKeyState('A') & 0x8000)
		{
			direction = "left";
		}
		if (GetKeyState('S') & 0x8000)
		{
			direction = "down";
		}
		if (GetKeyState('D') & 0x8000)
		{
			direction = "right";
		}

		system("cls");

		// print out the plane
		for (int y{}; y < yLimit; y++)
		{
			char currentChar{};

			for (int x{}; x < xLimit; x++)
			{
				currentChar = plane[x][y];

				if (currentChar != '\0')
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

		// run at desired (60) fps
		Sleep(17);
	}

	return 0;
}