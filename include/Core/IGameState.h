#pragma once
#include <SDL.h>

class GameStateManager; // 前方宣言
class UIManager;        // 前方宣言

class IGameState {
public:
    virtual ~IGameState() {} // 仮想デストラクタ（超重要）

    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual void ProcessInput() = 0;
    virtual void Update(float dt, GameStateManager* gsm) = 0;
    virtual void Render(SDL_Renderer* renderer, UIManager* uiManager) = 0;
};