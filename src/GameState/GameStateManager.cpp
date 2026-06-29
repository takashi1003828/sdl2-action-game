#include "Core/GameStateManager.h"

GameStateManager::GameStateManager() : currentState(nullptr) {}

GameStateManager::~GameStateManager() {
    if (currentState != nullptr) {
        currentState->OnExit();
        delete currentState;
    }
}

void GameStateManager::ChangeState(IGameState* newState) {
    if (currentState != nullptr) {
        currentState->OnExit();
        delete currentState;
    }
    currentState = newState;
    if (currentState != nullptr) {
        currentState->OnEnter();
    }
}

void GameStateManager::ProcessInput() {
    if (currentState) currentState->ProcessInput();
}

void GameStateManager::Update(float dt) {
    if (currentState) currentState->Update(dt, this);
}

void GameStateManager::Render(SDL_Renderer* renderer, UIManager* uiManager) {
    if (currentState) currentState->Render(renderer, uiManager);

}