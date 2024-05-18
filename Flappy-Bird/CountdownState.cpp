#include "CountdownState.h"
#include "Game.h"
#include <string>

CountdownState::CountdownState(class Game* game)
	:State(game), countdownTimer(3.0f)
{}

void CountdownState::Enter()
{
	countdownTimer = 3.0f;
}

void CountdownState::Exit()
{

}

void CountdownState::Update(float deltaTime)
{
	countdownTimer -= deltaTime;
	if (countdownTimer <= 0.0f)
	{
		mGame->StartGame();
	}
}

void CountdownState::Render(SDL_Renderer* renderer)
{
	int seconds = (int)countdownTimer + 1;
	std::string text = std::to_string(seconds);
	mGame->WriteText(Game::FontType::HUGE, text.c_str(), VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2, true);
}