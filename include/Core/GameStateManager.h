#pragma once
#include "IGameState.h"

class GameStateManager {
private:
    IGameState* currentState;

public:
    GameStateManager();
    ~GameStateManager();

    void ChangeState(IGameState* newState);

    void ProcessInput();
    void Update(float dt);
    void Render(SDL_Renderer* renderer, UIManager* uiManager);
};