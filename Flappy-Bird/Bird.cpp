#include "Bird.h"

Bird::Bird()
{

}

Bird::Bird(SDL_Texture* texture, float x, float y)
	:mTexture(texture), x(x), y(y)
{
	mRect.x = x;
	mRect.y = y;
	SDL_QueryTexture(mTexture, NULL, NULL, &mRect.w, &mRect.h);
}

Bird::~Bird()
{

}

void Bird::Jump()
{
	dy = -5;
}

void Bird::Update(float deltaTime)
{
	dy += GRAVITY_STRENGTH * deltaTime;

	y += dy;
}

void Bird::Render(SDL_Renderer* renderer)
{
	mRect.x = x;
	mRect.y = y;
	SDL_RenderCopy(renderer, mTexture, NULL, &mRect);
}

bool Bird::Collides(const SDL_Rect other)
{
	if (mRect.x + 2 + mRect.w - 4 >= other.x && mRect.x <= other.x + other.w &&
		mRect.y + 2 + mRect.h - 4 >= other.y && mRect.y + 2 <= other.y + other.h)
	{
		return true;
	}
	else
	{
		return false;
	}
}