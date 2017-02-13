#include "classes.h"

Ball::Ball()
{
	_ball.x = (SCREEN_WIDTH / 2) - (BALL_WIDTH / 2);
	_ball.y = (SCREEN_HEIGHT / 2) - (BALL_HEIGHT / 2);
	_ball.w = BALL_WIDTH;
	_ball.h = BALL_HEIGHT;

	_bCollider.w = BALL_WIDTH;
	_bCollider.h = BALL_HEIGHT;

	BALL_VEL = 7.50;
	BALL_ANGLE = 45.0;

	xVel = 0.0; yVel = 0.0;
}

void Ball::moveToStartPos()
{
	_ball.x = (SCREEN_WIDTH / 2) - (BALL_WIDTH / 2);
	_ball.y = (SCREEN_HEIGHT / 2) - (BALL_HEIGHT / 2);
	_bCollider.x = _ball.x;
	_bCollider.y = _ball.y;
}

void Ball::handlePaddleCollision()
{
	//Check if there is collision with paddle
	_ball.x -= xVel;
	_bCollider.x = _ball.x;
	_ball.y -= yVel;
	_bCollider.y = _ball.y;

	xVel = -xVel * 2.15 * cos(BALL_ANGLE);
	yVel = yVel * 1.15 * sin(BALL_ANGLE);

}

bool Ball::move()
{
	//Moves ball left or right
	_ball.x += xVel;
	_bCollider.x = _ball.x;

	//Moves ball up or down
	_ball.y += yVel;
	_bCollider.y = _ball.y;

	//Ball moves too far up or down
	if (_ball.y < 0 || _ball.y + BALL_HEIGHT > SCREEN_HEIGHT)
	{
		_ball.y -= yVel;
		_bCollider.y = _ball.y;
		yVel = -yVel * sin(BALL_ANGLE);
		return true;
	}
	return false;
}

void Ball::resetVelocity()
{
	if (xVel < 0)
		xVel = -BALL_VEL;
	else
		xVel = BALL_VEL;

	if (yVel < 0)
		yVel = -BALL_VEL;
	else
		yVel = BALL_VEL;
	
	//Inverse x velocity if either player has scored
	xVel = -xVel;
}

SDL_Rect* Ball::getCollider()
{
	return &_bCollider;
}

void Ball::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect( renderer, &_ball);
}

