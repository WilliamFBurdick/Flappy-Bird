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
	enum FontType
	{
		FLAPPY, SMALL, MEDIUM, HUGE
	};
	enum SoundEffect
	{
		JUMP, HURT, EXPLOSION, SCORE
	};
	Game();
	~Game();
	void Run();
	std::vector<Pipe*>& GetPipes() { return pipes; }
	const Uint8* GetKeyboard() { return mKeyboard; }

	void SetBird(Bird* bird) { this->bird = bird; }
	Bird* GetBird() { return bird; }

	void SetPipeTex(SDL_Texture* tex) { pipeTex = tex; }
	SDL_Texture* GetPipeTex() { return pipeTex; }
	SDL_Texture* LoadImage(const char* path);

	int GetLastY() { return lastY; }
	void SetLastY(int newY) { lastY = newY; }

	int GetPipeHeight() { return pipeHeight; }
	void SetPipeHeight(int newHeight) { pipeHeight = newHeight; }

	bool GetScrolling() { return scrolling; }
	void SetScrolling(bool value) { scrolling = value; }

	float GetPipeSpawnTimer() { return pipeSpawnTimer; }
	void SetPipeSpawnTimer(float value) { pipeSpawnTimer = value; }

	void EndGame(int score);
	void StartGame();
	void StartCountdown();
	void ReturnToMenu();

	void WriteText(FontType font, const char* text, float x, float y, bool centerAlign, SDL_Color color = { 0xFF, 0xFF, 0xFF, 0xFF });
	void PlaySound(SoundEffect effect);
private:
	void Input();
	void Update(float deltaTime);
	void Render();

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

	Mix_Chunk* mJumpSound;
	Mix_Chunk* mHurtSound;
	Mix_Chunk* mExplosionSound;
	Mix_Chunk* mScoreSound;
	Mix_Music* mMusic;

	Bird* bird;
	SDL_Texture* pipeTex;
	std::vector<Pipe*> pipes;
	float pipeSpawnTimer;

	class StateMachine* mStateMachine;

	int lastY;
	int pipeHeight;
	bool scrolling;
};

