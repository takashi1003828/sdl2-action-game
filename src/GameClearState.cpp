#include "Core/GameClearState.h"
#include "Core/PlayState.h"
#include "Core/InputManager.h"
#include "Core/GameStateManager.h"
#include "Core/UIManager.h"
#include <iostream>

GameClearState::GameClearState() {}
GameClearState::~GameClearState() {}

void GameClearState::OnEnter() {
    std::cout << "ゲームクリア画面に遷移しました" << std::endl;
}

void GameClearState::OnExit() {}
void GameClearState::ProcessInput() {}

void GameClearState::Update(float dt, GameStateManager* gsm) {
    // Rキーが押されたらPlayState（ゲーム最初）に戻る
    auto& input = InputManager::GetInstance();
    if (input.IsKeyDown(SDL_SCANCODE_R)) {
        gsm->ChangeState(new PlayState());
    }
}

void GameClearState::Render(SDL_Renderer* renderer, UIManager* uiManager) {
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderClear(renderer);

    if (uiManager) {
        uiManager->DrawGameClear(renderer);
    }
}