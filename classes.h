#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

enum GameState { PLAYING, PAUSED, EXIT };

class Ball
{
public:
	//Ball dimensions
	const int BALL_WIDTH = 20;
	const int BALL_HEIGHT = 20;

	//Ball's velocity
	double BALL_VEL;

	//Ball's angle
	double BALL_ANGLE;

	//Constructor
	Ball();

	//Resets ball's velocity
	void resetVelocity();

	//Handles collision with paddle
	void handlePaddleCollision();

	//Moves the ball
	bool move();

	//Move ball to its starting position
	void moveToStartPos();

	//Get ball's collider box
	SDL_Rect* getCollider();

	//Renders the ball
	void render(SDL_Renderer* renderer);

private:
	SDL_Rect _ball;

	double xVel;
	double yVel;

	SDL_Rect _bCollider;
};

class Paddle
{
	public:
		//Paddle dimensions
		const int PADDLE_WIDTH = 20;
		const int PADDLE_HEIGHT = 80;

		//Paddle's velocity
		const int PADDLE_VEL = 10;

		//Constructor
		Paddle();

		//Genereate id for a particular paddle
		void generateID(int id);

		//Sets inittial position
		void setInitPos(int x, int y);

		//Handles events on the paddle
		void handleEvents(SDL_Event* e);

		//Moves the paddle
		virtual void move(SDL_Rect* ball_collider, int boundary);

		//Returns paddle's collision box
		SDL_Rect* getCollider();

		//Renders the paddle
		void render(SDL_Renderer* renderer);

	protected:
		SDL_Rect _paddle;

		int _yVel;

		SDL_Rect _pCollider;

	private:

		int _paddleID;
		
};

class AIPaddle : public Paddle
{
public:
	//Constructor
	AIPaddle();

	//Tracks ball to move AI Paddle accordingly
	void move(SDL_Rect* ball_collider, int boundary);
};

class Texture
{
public:
	//Initializer
	Texture();

	//Deallocate memory
	~Texture();

	//Creates texture from font string
	bool loadText(TTF_Font* font, SDL_Renderer* renderer, std::string text);

	//Deallocates texture
	void free_texture();

	//Renders the texture
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL);

	//Returns image dimensions
	int getWidth();
	int getHeight();
private:
	//Actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class MainGame
{
public:
	//Initializes SDL subsystems
	bool initSystems();

	//Sets up the game
	bool loadAssets();

	//Runs the game
	void runGame(std::string gameMode, std::string gameDif);

	//Quits SDL subsystems
	void close();

private:
	SDL_Window* _window;

	SDL_Renderer* _renderer;

	TTF_Font* _font;

	Mix_Chunk* _wallSound;

	Mix_Chunk* _paddleSound;

	Mix_Chunk* _scoreSound;

	GameState _gameState;

	bool start;

	//Player 1 score
	int _p1Score;

	//Player 2 score
	int _p2Score;

	//Handle events when game is running
	void handleGameEvents(SDL_Event* e, Paddle* paddle1, Paddle* paddle2, Ball* ball, Texture* score1, Texture* score2, Texture* winPrompt);

	//Render game objects
	void renderObj(Paddle* paddle1, Paddle* paddle2, Ball* ball, Texture* score1, Texture* score2, Texture* winPrompt, Texture* pauseMessage);

	//Prompts win message
	void renderWinMessage(Texture* winPrompt, std::string player);

	//Check if there is collision between ball and paddle
	bool checkCollision(SDL_Rect* paddle, SDL_Rect* ball);

	//Resets the game
	void reset(Texture* score1, Texture* score2, Texture* winPrompt);
};
