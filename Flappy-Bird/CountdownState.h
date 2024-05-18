#pragma once
#include "State.h"
class CountdownState : public State
{
public:
	CountdownState(class Game* game);
	~CountdownState();

	virtual void Update(float deltaTime) override;
	virtual void Render(SDL_Renderer* renderer) override;

	virtual void Enter() override;
	virtual void Exit() override;
private:
	float countdownTimer;
};

