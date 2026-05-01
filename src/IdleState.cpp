#include "Entities/States/IdleState.h"
#include "Entities/States/WalkState.h" 
#include "Entities/States/JumpState.h"
#include "Entities/States/AirState.h"
#include "Entities/Player.h"
#include "Core/InputManager.h"
#include "Core/Constants.h"

void IdleState::Enter(Player* player) {

}

void IdleState::Update(Player* player, float dt, const std::vector<SDL_Rect>& colliders) {

    // プレイヤーが左右の移動キーを押しているかチェック
    auto& input = InputManager::GetInstance();
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT) ||
        input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        // プレイヤーに対して「歩き状態」への変更を指示する！
        player->ChangeState(new WalkState());
        return; // 状態が変わったので、このフレームのUpdate処理はここで終了
    }
    
    // もしジャンプボタンが押されていて、かつ地面にいるなら
    if (input.IsKeyDown(SDL_SCANCODE_SPACE) && player->Grounded(colliders)) {
        player->ChangeState(new JumpState());
        return;
    }

    //もしジャンプボタンが離されていて、かつ空中にいるなら
    if (!input.IsKeyDown(SDL_SCANCODE_SPACE) && !player->Grounded(colliders)) {
        player->ChangeState(new AirState());
        return;
    }
}

void IdleState::Exit(Player* player) {
    // 待機状態から抜けるときの処理
}