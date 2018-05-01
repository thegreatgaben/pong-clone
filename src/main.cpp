#include "classes.h"


int main(int argc, char* args[])
{
	std::cout << "WELCOME TO PONG!!!" << std::endl;
	
	std::string gameMode;
	std::string difficulty;
	while (true)
	{
		std::cout << std::endl << std::endl << "Single player or multiplayer?: ";
		std::cin >> gameMode;

		if (gameMode == "s" || gameMode == "single" || gameMode == "singleplayer" || gameMode == "single player" ||
			gameMode == "m" || gameMode == "multi" || gameMode == "multiplayer")
		{
			//Player choose single player game
			if (gameMode[0] == 's')
			{
				//Shows difficulty options
				std::cout << std::endl << "Choose the difficulty:" << std::endl;
				std::cout << "-Easy" << std::endl;
				std::cout << "-Normal" << std::endl;
				std::cout << "-Hard" << std::endl;
				std::cout << "-IMPOSSIBLE" << std::endl;
				std::cin >> difficulty;

				if (difficulty == "e" || difficulty == "easy" || difficulty == "n" || difficulty == "normal" ||
					difficulty == "h" || difficulty == "hard" || difficulty == "i" || difficulty == "impossible")
				{
					break;
				}
			}
			//Player chooses multiplayer game
			else
			{
				difficulty = "NULL";
				break;
			}
		}
		else
		{
			std::cout << std::endl << "I do not understand that! Please give a valid input!" << std::endl;
		}
	}
	
	MainGame game;

	if (game.initSystems())
	{
		if (game.loadAssets())
		{
			game.runGame(gameMode, difficulty);
		}
	}

	game.close();

	return 0;
}