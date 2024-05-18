#pragma once

#include <SDL.h>

class State
{
public:
	State(class Game* game)
		:mGame(game) {}
	~State() {}

	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer) = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;
protected:
	class Game* mGame;
};

