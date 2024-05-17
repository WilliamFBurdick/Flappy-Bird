#include "Pipe.h"

Pipe::Pipe()
{

}

Pipe::Pipe(SDL_Texture* tex, float x, float y)
	:x(x), y(y), mTexture(tex)
{
	mTopRect = SDL_Rect();
	mTopRect.x = x;
	mTopRect.y = y;
	SDL_QueryTexture(mTexture, NULL, NULL, &mTopRect.w, &mTopRect.h);
	mBottomRect.w = mTopRect.w;
	mBottomRect.h = mTopRect.h;

	remove = false;
}

Pipe::~Pipe()
{
	
}

void Pipe::Render(SDL_Renderer* renderer)
{
	mTopRect.x = x;
	mTopRect.y = y;

	mBottomRect.x = x;
	mBottomRect.y = y + PIPE_GAP + mBottomRect.h;

	SDL_RenderCopy(renderer, mTexture, NULL, &mBottomRect);
	SDL_RenderCopyEx(renderer, mTexture, NULL, &mTopRect, 0.0, NULL, SDL_FLIP_VERTICAL);
}

void Pipe::Update(float deltaTime)
{
	x += PIPE_SCROLL;
}