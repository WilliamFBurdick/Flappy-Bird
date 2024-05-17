#include "Game.h"
#include <stdexcept>
#include <cmath>

int lastY;
int pipeHeight;
bool scrolling;

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

	SDL_Texture* birdTex = LoadImage("Assets/bird.png");
	int birdTexW, birdTexH;
	SDL_QueryTexture(birdTex, NULL, NULL, &birdTexW, &birdTexH);
	bird = Bird(birdTex, VIRTUAL_WIDTH / 2 - (birdTexW / 2), VIRTUAL_HEIGHT / 2 - (birdTexH / 2));

	// Load pipe texture
	pipeTex = LoadImage("Assets/pipe.png");
	// Get pipe height
	SDL_QueryTexture(pipeTex, NULL, NULL, NULL, &pipeHeight);

	// Create pipe
	lastY = -pipeHeight + rand() % 80 + 20;
	pipes.push_back(new Pipe(pipeTex, VIRTUAL_WIDTH, lastY));
	
	// Load fonts
	mSmallFont = TTF_OpenFont("Assets/font.ttf", 8);
	mMediumFont = TTF_OpenFont("Assets/flappy.ttf", 14);
	mFlappyFont = TTF_OpenFont("Assets/flappy.ttf", 28);
	mHugeFont = TTF_OpenFont("Assets/flappy.ttf", 56);

	mKeyboard = nullptr;
	mRunning = false;

	scrolling = true;
}

Game::~Game()
{
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
				bird.Jump();
			}
			break;
		}
	}
	mKeyboard = SDL_GetKeyboardState(NULL);
}

void Game::Update(float deltaTime)
{
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

		// Update bird
		bird.Update(deltaTime);

		// Update pipe spawning
		pipeSpawnTimer += deltaTime;
		if (pipeSpawnTimer >= 2.0f)
		{
			lastY = std::max(-pipeHeight + 10, std::min(lastY + rand() % 40 - 20, VIRTUAL_HEIGHT - 90 - pipeHeight));
			pipes.push_back(new Pipe(pipeTex, VIRTUAL_WIDTH, lastY));
			pipeSpawnTimer = 0.0f;
		}

		// Update the pipes
		for (auto it = pipes.begin(); it != pipes.end(); ++it)
		{
			(*it)->Update(deltaTime);
			if ((*it)->GetX() < -(*it)->GetWidth())
			{
				(*it)->SetRemove(true);
			}

			if (bird.Collides((*it)->GetTopRect()) || bird.Collides((*it)->GetBottomRect()))
			{
				scrolling = false;
			}
		}

		// Remove pipes that are flagged for removal
		for (auto it = pipes.begin(); it != pipes.end(); ++it)
		{
			if ((*it)->GetRemove())
			{
				it = pipes.erase(it);
			}
		}

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
	bird.Render(mRenderer);


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