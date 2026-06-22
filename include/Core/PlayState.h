#pragma once
#include "IGameState.h"
#include "Entities/Player.h"
#include "Core/Map.h"
#include "Core/Camera.h"
#include <vector>

class PlayState : public IGameState {
private:
    Player player;
    Map myMap;
    Camera* camera;

public:
    PlayState();
    ~PlayState() override; // overrideキーワードを書くと安全です

    void OnEnter() override;
    void OnExit() override;
    void ProcessInput() override;
    void Update(float dt, GameStateManager* gsm) override;
    void Render(SDL_Renderer* renderer, UIManager* uiManager) override;
};