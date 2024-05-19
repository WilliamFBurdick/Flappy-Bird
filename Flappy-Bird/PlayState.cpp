#include "PlayState.h"
#include "Game.h"
#include <string>

PlayState::PlayState(Game* game)
	:State(game)
{

}

void PlayState::Update(float deltaTime)
{

	// Update bird
	mGame->GetBird()->Update(deltaTime);

	// Update pipe spawning
	mGame->SetPipeSpawnTimer(mGame->GetPipeSpawnTimer() + deltaTime);
	if (mGame->GetPipeSpawnTimer() >= 2.0f)
	{
		mGame->SetLastY(std::max(-mGame->GetPipeHeight() + 10, std::min(mGame->GetLastY() + rand() % 40 - 20, VIRTUAL_HEIGHT - 90 - mGame->GetPipeHeight())));
		mGame->GetPipes().push_back(new Pipe(mGame->GetPipeTex(), VIRTUAL_WIDTH, mGame->GetLastY()));
		mGame->SetPipeSpawnTimer(0.0f);
	}

	// Update the pipes
	for (int i = 0; i < mGame->GetPipes().size(); i++)
	{
		Pipe* pipe = mGame->GetPipes()[i];
		pipe->Update(deltaTime);
		if (pipe->GetX() < -pipe->GetWidth())
		{
			pipe->SetRemove(true);
		}

	 	if (mGame->GetBird()->Collides(pipe->GetTopRect()) || mGame->GetBird()->Collides(pipe->GetBottomRect()) ||
			mGame->GetBird()->GetRect().y > VIRTUAL_HEIGHT - mGame->GetBird()->GetRect().h)
		{
			mGame->PlaySound(Game::SoundEffect::HURT);
			mGame->PlaySound(Game::SoundEffect::EXPLOSION);
			mGame->SetScrolling(false);
			mGame->EndGame(score);
			return;
		}

		if (!pipe->GetScored() && pipe->GetX() + pipe->GetTopRect().w < mGame->GetBird()->GetRect().x)
		{
			mGame->PlaySound(Game::SoundEffect::SCORE);
			score += 1;
			pipe->SetScored(true);
		}
	}

	// Remove pipes that are flagged for removal
	for (int i = 0; i < mGame->GetPipes().size(); i++)
	{
		Pipe* pipe = mGame->GetPipes()[i];
		if (pipe->GetRemove())
		{
			mGame->GetPipes().erase(mGame->GetPipes().begin() + i);
		}
	}
}

void PlayState::Render(SDL_Renderer* renderer)
{
	std::string text = "Score: " + std::to_string(score);
	mGame->WriteText(Game::FontType::MEDIUM, text.c_str(), 8, 8, false);
}

void PlayState::Enter()
{
	SDL_Texture* birdTex = mGame->LoadImage("Assets/bird.png");
	int birdTexW, birdTexH;
	SDL_QueryTexture(birdTex, NULL, NULL, &birdTexW, &birdTexH);
	mGame->SetBird(new Bird(birdTex, VIRTUAL_WIDTH / 2 - (birdTexW / 2), VIRTUAL_HEIGHT / 2 - (birdTexH / 2)));

	// Load pipe texture
	mGame->SetPipeTex(mGame->LoadImage("Assets/pipe.png"));
	// Get pipe height
	int pipeHeight;
	SDL_QueryTexture(mGame->GetPipeTex(), NULL, NULL, NULL, &pipeHeight);
	mGame->SetPipeHeight(pipeHeight);

	// Create pipe
	mGame->SetLastY(-pipeHeight + rand() % 80 + 20);
	mGame->GetPipes().push_back(new Pipe(mGame->GetPipeTex(), VIRTUAL_WIDTH, mGame->GetLastY()));
	// Reset pipe spawn timer
	mGame->SetPipeSpawnTimer(0.0f);

	mGame->SetScrolling(true);
}

void PlayState::Exit()
{
	// Clean  up the state by removing everything
	mGame->GetPipes().clear();
	delete mGame->GetBird();
	mGame->SetBird(nullptr);
}