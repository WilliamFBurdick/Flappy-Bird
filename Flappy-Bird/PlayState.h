#pragma once

#include "State.h"

class PlayState : public State
{
public:
	PlayState(class Game* game);
	~PlayState();

	virtual void Update(float deltaTime) override;
	virtual void Render(SDL_Renderer* renderer) override;
	virtual void Enter() override;
	virtual void Exit() override;
private:
	int score = 0;
};

