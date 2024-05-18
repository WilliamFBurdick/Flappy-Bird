#include "StateMachine.h"
#include "State.h"

StateMachine::StateMachine()
{
	mCurrentState = nullptr;
}

void StateMachine::Update(float deltaTime)
{
	if (mCurrentState != nullptr)
		mCurrentState->Update(deltaTime);
}

void StateMachine::Render(SDL_Renderer* renderer)
{
	if (mCurrentState != nullptr)
		mCurrentState->Render(renderer);
}

void StateMachine::ChangeState(State* newState)
{
	if (mCurrentState != nullptr)
		mCurrentState->Exit();
	mCurrentState = newState;
	if (mCurrentState != nullptr)
		mCurrentState->Enter();
}