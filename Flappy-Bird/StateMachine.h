#pragma once
#include <SDL.h>

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer);
	void ChangeState(class State* newState);
private:
	class State* mCurrentState;
};

