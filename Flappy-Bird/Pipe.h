#pragma once

#include <SDL.h>

const int PIPE_SCROLL = -3;
const int PIPE_GAP = 90;

class Pipe
{
public:
	Pipe();
	Pipe(SDL_Texture* tex, float x, float y);
	~Pipe();
	void Render(SDL_Renderer* renderer);
	void Update(float deltaTime);

	float GetX() { return x; }
	float GetWidth() { return mTopRect.w; }

	bool GetRemove() { return remove; }
	void SetRemove(bool value) { remove = value; }

	SDL_Rect GetTopRect() { return mTopRect; }
	SDL_Rect GetBottomRect() { return mBottomRect; }
private:
	bool remove;
	float x, y;
	SDL_Rect mTopRect, mBottomRect;
	SDL_Texture* mTexture;
};

