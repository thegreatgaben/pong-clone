#include "classes.h"

Paddle::Paddle()
{
	_paddle.x = 0;
	_paddle.y = 0;
	_paddle.w = PADDLE_WIDTH;
	_paddle.h = PADDLE_HEIGHT;

	_pCollider.w = PADDLE_WIDTH;
	_pCollider.h = PADDLE_HEIGHT;

	_paddleID = NULL;

	_yVel = 0;
}

void Paddle::generateID(int id)
{
	_paddleID = id;
}

void Paddle::setInitPos(int x, int y)
{
	_paddle.x = x;
	_paddle.y = y;
	_pCollider.x = _paddle.x;
	_pCollider.y = _paddle.y;
}

void Paddle::handleEvents(SDL_Event* e)
{
	if (_paddleID == 0)
	{
		//If a key was pressed
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_w: _yVel -= PADDLE_VEL; break;
			case SDLK_s: _yVel += PADDLE_VEL; break;
			}
		}
		//If a key was released, undo velocity change
		else if (e->type == SDL_KEYUP && e->key.repeat == 0)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_w: _yVel += PADDLE_VEL; break;
			case SDLK_s: _yVel -= PADDLE_VEL; break;
			}
		}
	}
	else if (_paddleID == 1)
	{
		//If a key was pressed
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_UP: _yVel -= PADDLE_VEL; break;
			case SDLK_DOWN: _yVel += PADDLE_VEL; break;
			}
		}
		//If a key was released, undo velocity change
		else if (e->type == SDL_KEYUP && e->key.repeat == 0)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_UP: _yVel += PADDLE_VEL; break;
			case SDLK_DOWN: _yVel -= PADDLE_VEL; break;
			}
		}
	}
}

void Paddle::move(SDL_Rect* ball_collider, int boundary)
{
	//Move the paddle up or down
	_paddle.y += _yVel;
	_pCollider.y = _paddle.y;

	//Paddle went too far up or down
	if (_paddle.y < 0 || (_paddle.y + PADDLE_HEIGHT > SCREEN_HEIGHT))
	{
		_paddle.y -= _yVel;
		_pCollider.y -= _yVel;
	}
}

SDL_Rect* Paddle::getCollider()
{
	return &_pCollider;
}

void Paddle::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect( renderer, &_paddle);
}


AIPaddle::AIPaddle()
{
	_paddle.x = 0;
	_paddle.y = 0;
	_paddle.w = PADDLE_WIDTH;
	_paddle.h = PADDLE_HEIGHT;

	_pCollider.w = PADDLE_WIDTH;
	_pCollider.h = PADDLE_HEIGHT;

	_yVel = 0;
}

void AIPaddle::move(SDL_Rect* ball_collider, int boundary)
{
	if (ball_collider->x > boundary)
	{
		if (ball_collider->y < _paddle.y)
			_yVel -= PADDLE_VEL;
		else if (ball_collider->y >(_paddle.y + _paddle.h))
			_yVel += PADDLE_VEL;
	}

	//Move the paddle up or down
	_paddle.y += _yVel;
	_pCollider.y = _paddle.y;

	//Paddle went too far up or down
	if (_paddle.y < 0 || (_paddle.y + PADDLE_HEIGHT > SCREEN_HEIGHT))
	{
		_paddle.y -= _yVel;
		_pCollider.y -= _yVel;
	}

	//Undo veloctiy change
	_yVel -= _yVel;
}