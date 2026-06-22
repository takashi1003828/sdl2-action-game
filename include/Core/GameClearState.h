// Core/GameClearState.h
#pragma once
#include "Core/IGameState.h" // Stateの基底クラス（名前は環境に合わせてください）

class GameClearState : public IGameState {
public:
    GameClearState();
    ~GameClearState() override;

    void OnEnter() override;
    void OnExit() override;
    void ProcessInput() override;
    void Update(float dt, GameStateManager* gsm) override;
    void Render(SDL_Renderer* renderer, UIManager* uiManager) override;
};