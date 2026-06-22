// Core/GameOverState.h
#pragma once
#include "Core/IGameState.h" // Stateの基底クラス（名前は環境に合わせてください）

class GameOverState : public IGameState {
public:
    GameOverState();
    ~GameOverState() override;

    void OnEnter() override;
    void OnExit() override;
    void ProcessInput() override;
    void Update(float dt, GameStateManager* gsm) override;
    void Render(SDL_Renderer* renderer, UIManager* uiManager) override;
};