#include "ScoreState.h"
#include "Game.h"
#include <string>

ScoreState::ScoreState(Game* game, int score)
	:State(game), score(score)
{}

void ScoreState::Update(float deltaTime)
{
	if (mGame->GetKeyboard()[SDL_SCANCODE_RETURN])
	{
		mGame->StartCountdown();
	}
}

void ScoreState::Render(SDL_Renderer* renderer)
{
	std::string finalScore = "FINAL SCORE: " + std::to_string(score);
	mGame->WriteText(Game::FontType::MEDIUM, finalScore.c_str(), VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2, true);
	mGame->WriteText(Game::FontType::SMALL, "Press ENTER to play again!", VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + 30, true);
}

void ScoreState::Enter()
{

}

void ScoreState::Exit()
{

}