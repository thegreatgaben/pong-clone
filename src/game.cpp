#include "classes.h"

bool MainGame::initSystems()
{
	bool status;
	status = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		status = false;
	}
	else
	{
		_window = SDL_CreateWindow("Pong",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		if (_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			status = false;
		}
		else
		{
			//Create renderer for window
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (_renderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				status = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					status = false;
				}

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					status = false;
				}
			}
		}
	}

	return status;
}

bool MainGame::loadAssets()
{
	bool status = true;

	_font = TTF_OpenFont("fonts/ARCADECLASSIC.TTF", 150);
	if (_font == NULL)
	{
		printf("Failed to load font! TTF error: %s\n", TTF_GetError());
		status = false;
	}
	else
	{
		//Load sound effects
		_wallSound = Mix_LoadWAV("sounds/ping_pong_8bit_plop.wav");
		if (_wallSound == NULL)
		{
			printf("Failed to load wall sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			status = false;
		}

		_paddleSound = Mix_LoadWAV("sounds/ping_pong_8bit_beeep.wav");
		if (_paddleSound == NULL)
		{
			printf("Failed to load paddle sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			status = false;
		}

		_scoreSound = Mix_LoadWAV("sounds/ping_pong_8bit_peeeeeep.wav");
		if (_scoreSound == NULL)
		{
			printf("Failed to load score sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			status = false;
		}

	}

	return status;
}

void MainGame::runGame(std::string gameMode, std::string gameDif)
{
	int boundary = NULL;

	Paddle _paddle1;
	Paddle* _paddle2 = new Paddle();
	//If player chooses single player game
	if (gameMode[0] == 's')
	{
		//Check difficulty as selected by player
		if (gameDif[0] == 'e')
			//Easy
			boundary = (SCREEN_WIDTH / 3) * 2;
		else if (gameDif[0] == 'n')
			//Normal
			boundary = SCREEN_WIDTH / 2;
		else if (gameDif[0] == 'h')
			//Hard
			boundary = SCREEN_WIDTH / 3;
		else if (gameDif[0] == 'i')
			//Impossible
			boundary = 0;

		AIPaddle* paddleAI = new AIPaddle();
		delete _paddle2;
		_paddle2 = nullptr;
		_paddle2 = paddleAI;
	}
	Ball _gBall;

	//Generate IDs for paddles
	_paddle1.generateID(0);
	_paddle2->generateID(1);

	//Set paddle positions
	_paddle1.setInitPos(_paddle1.PADDLE_WIDTH / 2, (SCREEN_HEIGHT / 2) - (_paddle1.PADDLE_HEIGHT / 2));
	_paddle2->setInitPos((SCREEN_WIDTH - (_paddle2->PADDLE_WIDTH)) - (_paddle2->PADDLE_WIDTH / 2), (SCREEN_HEIGHT / 2) - (_paddle1.PADDLE_HEIGHT / 2));

	//Sets initial velocity for ball
	_gBall.resetVelocity();

	//Initialize score and start flag
	start = false;
	_p1Score = 0;
	_p2Score = 0;

	//Create textures
	Texture score1;
	Texture score2;
	Texture winPrompt;
	Texture pauseMessage;
	
	//Load pause text for pause message texture
	pauseMessage.loadText(_font, _renderer, "Paused!");

	//Initialize player score
	std::string scoreStr = "0";
	score1.loadText(_font, _renderer, scoreStr);
	score2.loadText(_font, _renderer, scoreStr);

	bool _p1Scored = false;
	bool _p2Scored = false;

	SDL_Event event;

	while (_gameState != EXIT)
	{
		//Handle game events
		handleGameEvents(&event, &_paddle1, _paddle2, &_gBall, &score1, &score2, &winPrompt);
		
		if (start)
		{
			//If ball collided with either paddle
			if (checkCollision(_paddle1.getCollider(), _gBall.getCollider()) || 
				checkCollision(_paddle2->getCollider(), _gBall.getCollider()))
			{
				_gBall.handlePaddleCollision();
				Mix_PlayChannel(-1, _paddleSound, 0);
			}
			else
			{
				//Ball moves pass either paddles
				if (_gBall.getCollider()->x < 5)
				{
					//Player 2 scores
					_gBall.resetVelocity();
					_gBall.moveToStartPos();
					_p2Score += 1;
					_p2Scored = true;
					Mix_PlayChannel(-1, _scoreSound, 0);
				}
				else if (_gBall.getCollider()->x + _gBall.BALL_WIDTH >(SCREEN_WIDTH - 5))
				{
					//Player 1 scores
					_gBall.resetVelocity();
					_gBall.moveToStartPos();
					_p1Score += 1;
					_p1Scored = true;
					Mix_PlayChannel(-1, _scoreSound, 0);
				}
			}

			bool hitWall = _gBall.move();
			if (hitWall)
			{
				//Ball hits wall
				Mix_PlayChannel(-1, _wallSound, 0);
			}

			//Move paddle if any changes were made
			_paddle1.move(_gBall.getCollider(), boundary);
			_paddle2->move(_gBall.getCollider(), boundary);
		}

		if (_p1Scored)
		{
			//Update player 1 score
			scoreStr = std::to_string(_p1Score);
			score1.loadText(_font, _renderer, scoreStr);
			_p1Scored = false;
		}
		else if (_p2Scored)
		{
			//Update player 2 score
			scoreStr = std::to_string(_p2Score);
			score2.loadText(_font, _renderer, scoreStr);
			_p2Scored = false;
		}

		//Render game objects
		renderObj(&_paddle1, _paddle2, &_gBall, &score1, &score2, &winPrompt, &pauseMessage);
	}

	delete _paddle2;
	_paddle2 = nullptr;
	score1.free_texture();
	score2.free_texture();
	winPrompt.free_texture();
	pauseMessage.free_texture();
}

void MainGame::handleGameEvents(SDL_Event* e, Paddle* paddle1, Paddle* paddle2, Ball* ball, Texture* score1, Texture* score2, Texture* winPrompt)
{
	while (SDL_PollEvent(e) != 0)
	{
		if (e->type == SDL_QUIT)
			_gameState = EXIT;
		else if (e->type == SDL_KEYDOWN)
		{
			switch (e->key.keysym.sym)
			{
				case SDLK_SPACE: 
					start = true;
					reset(score1, score2, winPrompt);
					ball->moveToStartPos();
					break;
				case SDLK_p:
				{
					//Game is already paused
					if (_gameState == PAUSED)
					{
						//Unpause the game
						_gameState = PLAYING;
						start = true;
					}
					else
					{
						//Pause the game
						_gameState = PAUSED;
						start = false;
					}
				} break;
				case SDLK_ESCAPE:
					_gameState = EXIT;
					break;
			}
		}

		//Handle paddle events
		paddle1->handleEvents(e);
		paddle2->handleEvents(e);
	}
}

void MainGame::renderObj(Paddle* paddle1, Paddle* paddle2, Ball* ball, Texture* score1, Texture* score2, Texture* winPrompt, Texture* pauseMessage)
{
	//Clear screen
	SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(_renderer);

	//Draw middle line
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawLine(_renderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);

	//Render scores
	score1->render(_renderer, (SCREEN_WIDTH / 4) - (score1->getWidth() / 2), 20);
	score2->render(_renderer, ((SCREEN_WIDTH / 4) * 3) - (score2->getWidth() / 2), 20);

	//Render the paddles
	paddle1->render(_renderer);
	paddle2->render(_renderer);

	//Render the ball
	ball->render(_renderer);

	//Render win message if either player has won
	if (_p1Score == 10)
	{
		renderWinMessage(winPrompt, "1");
	}
	else if (_p2Score == 10)
	{
		renderWinMessage(winPrompt, "2");
	}

	//If the game is paused
	if (_gameState == PAUSED)
	{
		//Render pause message
		int x = (SCREEN_WIDTH / 2) - (pauseMessage->getWidth() / 2) + 40;
		int y = ( (SCREEN_HEIGHT / 2) - (pauseMessage->getHeight() / 2) ) - 20;
		pauseMessage->render(_renderer, x, y);
	}

	//Update screen
	SDL_RenderPresent(_renderer);
}

void MainGame::renderWinMessage(Texture* winPrompt, std::string player)
{
	int x = 0, y = 0;

	std::string str = "Player ";
	str += player;

	winPrompt->loadText( _font, _renderer, str);
	x = (SCREEN_WIDTH / 2) - (winPrompt->getWidth() / 2) + 20;
	y = ((SCREEN_HEIGHT / 2) - (winPrompt->getHeight() / 2)) - 20;
	winPrompt->render(_renderer, x, y);

	winPrompt->loadText( _font, _renderer, "Wins!");
	x = (SCREEN_WIDTH / 2) - (winPrompt->getWidth() / 2) + 20;
	y = (SCREEN_HEIGHT / 2);
	winPrompt->render(_renderer, x, y);
	start = false;
}

bool MainGame::checkCollision(SDL_Rect* paddle, SDL_Rect* ball)
{
	//The sides of the collision boxes
	int leftP, leftB;
	int rightP, rightB;
	int topP, topB;
	int bottomP, bottomB;

	leftP = paddle->x;
	rightP = paddle->x + paddle->w;
	topP = paddle->y;
	bottomP = paddle->y + paddle->h;

	leftB = ball->x;
	rightB = ball->x + ball->w;
	topB = ball->y;
	bottomB = ball->y + ball->h;

	//Check if the boxes are outside of each other
	if (bottomP <= topB)
		return false;

	if (topP >= bottomB)
		return false;

	if (rightP <= leftB)
		return false;

	if (leftP >= rightB)
		return false;

	//Both collision boxes' axis have intersect one another, hence collision
	return true;
}

void MainGame::reset(Texture* score1, Texture* score2, Texture* winPrompt)
{
	_p1Score = 0;
	_p2Score = 0;
	score1->loadText( _font, _renderer, "0");
	score2->loadText( _font, _renderer, "0");
	winPrompt->free_texture();
}

void MainGame::close()
{
	//Close font
	TTF_CloseFont(_font);
	_font = NULL;

	//Free sound effects
	Mix_FreeChunk(_wallSound);
	Mix_FreeChunk(_paddleSound);
	Mix_FreeChunk(_scoreSound);
	_wallSound = NULL;
	_paddleSound = NULL;
	_scoreSound = NULL;

	//Destroy window and renderer
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	_window = NULL;
	_renderer = NULL;

	//Quit SDL and its subsystems
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
}
