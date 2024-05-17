#include <stdexcept>
#include <stdio.h>
#include "Game.h"
#include <time.h>

int main(int argc, char* argv[])
{
	try
	{
		srand(time(NULL));
		Game game;
		game.Run();
	}
	catch (std::exception e)
	{
		printf("EXCEPTION: %s\n", e.what());
	}
	return 0;
}