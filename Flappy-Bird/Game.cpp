#include "Game.h"
#include <stdexcept>
#include <cmath>

#include "StateMachine.h"
#include "TitleScreenState.h"
#include "PlayState.h"
#include "ScoreState.h"
#include "CountdownState.h"

Game::Game()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL Initialization failed: %s!\n", SDL_GetError());
		throw std::exception("SDL Init Error!");
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		printf("SDL_image Initialization failed: %s\n", IMG_GetError());
		throw std::exception("SDL_image Init Error!");
	}
	if (TTF_Init() < 0)
	{
		printf("SDL_ttf Initialization failed: %s\n", TTF_GetError());
		throw std::exception("SDL_ttf Initialization Error");
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer Initialization failed: %s\n", Mix_GetError());
		throw std::exception("SDL_mixer Initialization Error");
	}

	mWindow = SDL_CreateWindow("Flappy Bird!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr)
	{
		printf("Failed to create SDL Window: %s\n", SDL_GetError());
		throw std::exception("SDL Error");
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr)
	{
		printf("Failed to create SDL Renderer: %s\n", SDL_GetError());
		throw std::exception("SDL Error");
	}

	// Set virtual resolution
	SDL_RenderSetLogicalSize(mRenderer, 512, 288);

	mBackground = Entity(LoadImage("Assets/background.png"));
	mGround = Entity(LoadImage("Assets/ground.png"));
	mGround.y = VIRTUAL_HEIGHT - mGround.rect.h;
	
	// Load fonts
	mSmallFont = TTF_OpenFont("Assets/font.ttf", 8);
	mMediumFont = TTF_OpenFont("Assets/flappy.ttf", 14);
	mFlappyFont = TTF_OpenFont("Assets/flappy.ttf", 28);
	mHugeFont = TTF_OpenFont("Assets/flappy.ttf", 56);

	// Load music
	mJumpSound = Mix_LoadWAV("Assets/jump.wav");
	mHurtSound = Mix_LoadWAV("Assets/hurt.wav");
	mExplosionSound = Mix_LoadWAV("Assets/explosion.wav");
	mScoreSound = Mix_LoadWAV("Assets/score.wav");
	mMusic = Mix_LoadMUS("Assets/marios_way.mp3");

	mKeyboard = nullptr;
	mRunning = false;

	scrolling = true;

	mStateMachine = new StateMachine();
	mStateMachine->ChangeState(new TitleScreenState(this));
}

Game::~Game()
{
	Mix_FreeChunk(mJumpSound);
	Mix_FreeChunk(mHurtSound);
	Mix_FreeChunk(mExplosionSound);
	Mix_FreeChunk(mScoreSound);
	Mix_FreeMusic(mMusic);

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

void Game::Run()
{
	mRunning = true;
	// Start music
	Mix_PlayMusic(mMusic, -1);
	float lastFrame = 0.0f;
	while (mRunning)
	{
		SDL_Delay(20);
		float currentFrame = SDL_GetTicks();
		float deltaTime = (currentFrame - lastFrame) / 1000.0f;
		lastFrame = currentFrame;
		Input();
		Update(deltaTime);
		Render();
	}
}

void Game::Input()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			mRunning = false;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				mRunning = false;
			}
			if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				if (bird != nullptr)
				{
					bird->Jump();
					PlaySound(SoundEffect::JUMP);
				}
			}
			break;
		}
	}
	mKeyboard = SDL_GetKeyboardState(NULL);
}

void Game::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
	// Background scrolling (state independent)
	if (scrolling)
	{
		// Update parallax scrolling
		mBackgroundScroll += BACKGROUND_SCROLL_SPEED;
		if (mBackgroundScroll >= BACKGROUND_LOOP_POINT)
			mBackgroundScroll = 0;
		mGroundScroll += GROUND_SCROLL_SPEED;
		if (mGroundScroll >= VIRTUAL_WIDTH)
			mGroundScroll = 0;

		mBackground.x = -mBackgroundScroll;
		mGround.x = -mGroundScroll;
	}
}

void Game::Render()
{
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	// Render background
	mBackground.Render(mRenderer);

	// Render the pipes
	for (auto e : pipes)
		e->Render(mRenderer);
	
	// Render the ground
	mGround.Render(mRenderer);

	// Render the bird
	if (bird != nullptr)
		bird->Render(mRenderer);

	mStateMachine->Render(mRenderer);

	SDL_RenderPresent(mRenderer);
}

SDL_Texture* Game::LoadImage(const char* path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* tempSurface = IMG_Load(path);
	if (!tempSurface)
	{
		printf("Failed to load image %s: %s!", path, IMG_GetError());
		return nullptr;
	}
	texture = SDL_CreateTextureFromSurface(mRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return texture;
}

void Game::EndGame(int score)
{
	mStateMachine->ChangeState(new ScoreState(this, score));
}

void Game::StartGame()
{
	mStateMachine->ChangeState(new PlayState(this));
}

void Game::ReturnToMenu()
{
	mStateMachine->ChangeState(new TitleScreenState(this));
}

void Game::StartCountdown()
{
	mStateMachine->ChangeState(new CountdownState(this));
}

void Game::WriteText(FontType font, const char* text, float x, float y, bool centerAlign, SDL_Color color)
{
	TTF_Font* textFont = nullptr;
	switch (font)
	{
	case FontType::FLAPPY:
		textFont = mFlappyFont;
		break;
	case FontType::SMALL:
		textFont = mSmallFont;
		break;
	case FontType::MEDIUM:
		textFont = mMediumFont;
		break;
	case FontType::HUGE:
		textFont = mHugeFont;
		break;
	default:
		textFont = mFlappyFont;
		break;
	}
	SDL_Surface* textSurf = TTF_RenderText_Solid(textFont, text, color);
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(mRenderer, textSurf);
	SDL_FreeSurface(textSurf);

	SDL_Rect rect;
	SDL_QueryTexture(textTex, NULL, NULL, &rect.w, &rect.h);

	if (centerAlign)
	{
		rect.x = x - rect.w / 2;
		rect.y = y - rect.h / 2;
	}
	else
	{
		rect.x = x;
		rect.y = y;
	}

	SDL_RenderCopy(mRenderer, textTex, NULL, &rect);
	SDL_DestroyTexture(textTex);
}

void Game::PlaySound(SoundEffect effect)
{
	Mix_Chunk* sound;
	switch (effect)
	{
	case JUMP:
		sound = mJumpSound;
		break;
	case HURT:
		sound = mHurtSound;
		break;
	case EXPLOSION:
		sound = mExplosionSound;
		break;
	case SCORE:
		sound = mScoreSound;
		break;
	default:
		sound = mJumpSound;
		break;
	}
	Mix_PlayChannel(-1, sound, 0);
}