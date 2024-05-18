#pragma once

#include "State.h"

class TitleScreenState : public State
{
public:
	TitleScreenState(class Game* game);
	~TitleScreenState();
	
	virtual void Update(float deltaTime) override;
	virtual void Render(SDL_Renderer* renderer) override;
	virtual void Enter() override;
	virtual void Exit() override;
};

