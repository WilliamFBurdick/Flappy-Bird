#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "Bird.h"
#include "Pipe.h"

const int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
const int VIRTUAL_WIDTH = 512, VIRTUAL_HEIGHT = 288;

const int BACKGROUND_SCROLL_SPEED = 1;
const int GROUND_SCROLL_SPEED = 3;

const int BACKGROUND_LOOP_POINT = 413;

struct Entity
{
	Entity() {}
	Entity(SDL_Texture* tex, float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
		this->tex = tex;
		SDL_QueryTexture(this->tex, NULL, NULL, &rect.w, &rect.h);
	}
	float x, y;
	SDL_Texture* tex;
	SDL_Rect rect;
	void Render(SDL_Renderer* renderer)
	{
		rect.x = static_cast<int>(x);
		rect.y = static_cast<int>(y);
		SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
		SDL_RenderCopy(renderer, tex, NULL, &rect);
	}
};

class Game
{
public:
	Game();
	~Game();
	void Run();
private:
	void Input();
	void Update(float deltaTime);
	void Render();

	SDL_Texture* LoadImage(const char* path);
private:
	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	const Uint8* mKeyboard;

	bool mRunning;

	Entity mBackground;
	Entity mGround;
	int mBackgroundScroll;
	int mGroundScroll;

	TTF_Font* mSmallFont;
	TTF_Font* mMediumFont;
	TTF_Font* mFlappyFont;
	TTF_Font* mHugeFont;

	Bird bird;
	SDL_Texture* pipeTex;
	std::vector<Pipe*> pipes;
	float pipeSpawnTimer;
};

