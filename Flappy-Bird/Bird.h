#pragma once

#include <SDL.h>

const int GRAVITY_STRENGTH = 20;

class Bird
{
public:
	Bird();
	Bird(SDL_Texture* texture, float x = 0, float y = 0);
	~Bird();

	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer);
	void Jump();

	void SetX(float x)
	{
		this->x = x;
		mRect.x = x;
	}

	void SetY(float y)
	{
		this->y = y;
		mRect.y = y;
	}

	SDL_Rect GetRect() { return mRect; }

	bool Collides(const SDL_Rect other);
private:
	float x, y;
	float dx, dy;
	SDL_Texture* mTexture;
	SDL_Rect mRect;
};

