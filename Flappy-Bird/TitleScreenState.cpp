#include "TitleScreenState.h"
#include "Game.h"

TitleScreenState::TitleScreenState(Game* game)
	:State(game)
{

}

void TitleScreenState::Update(float deltaTime)
{
	if (mGame->GetKeyboard()[SDL_SCANCODE_RETURN])
	{
		mGame->StartCountdown();
	}
}

void TitleScreenState::Render(SDL_Renderer* renderer)
{
	mGame->WriteText(Game::FontType::FLAPPY, "Flappy Bird!", VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2, true);
	mGame->WriteText(Game::FontType::SMALL, "Press ENTER to start!", VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + 30, true);
}

void TitleScreenState::Enter()
{

}

void TitleScreenState::Exit()
{

}