#pragma once
#include "State.h"
class ScoreState : public State
{
public:
	ScoreState(class Game* game, int score);
	~ScoreState();

	virtual void Update(float deltaTime) override;
	virtual void Render(SDL_Renderer* renderer) override;
	virtual void Enter() override;
	virtual void Exit() override;
private:
	int score;
};

