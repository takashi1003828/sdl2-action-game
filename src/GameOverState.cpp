#include "Core/GameOverState.h"
#include "Core/PlayState.h"
#include "Core/InputManager.h"
#include "Core/GameStateManager.h"
#include "Core/UIManager.h"
#include <iostream>

GameOverState::GameOverState() {}
GameOverState::~GameOverState() {}

void GameOverState::OnEnter() {
    std::cout << "ゲームオーバー画面に遷移しました" << std::endl;
}

void GameOverState::OnExit() {}
void GameOverState::ProcessInput() {}

void GameOverState::Update(float dt, GameStateManager* gsm) {
    // Rキーが押されたらPlayState（ゲーム最初）に戻る
    auto& input = InputManager::GetInstance();
    if (input.IsKeyDown(SDL_SCANCODE_R)) {
        gsm->ChangeState(new PlayState());
    }
}

void GameOverState::Render(SDL_Renderer* renderer, UIManager* uiManager) {
    // 背景を血のような赤色に
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderClear(renderer);

    // UIマネージャーを使って「GAME OVER」などのテキストを描画
    // 点滅させる演出などもここで書けます
    if (uiManager) {
        uiManager->DrawGameOver(renderer);
    }
}