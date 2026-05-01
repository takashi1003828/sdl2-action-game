#include "Entities/States/WalkState.h"
#include "Entities/States/IdleState.h"
#include "Entities/States/JumpState.h"
#include "Entities/States/DashState.h"
#include "Core/Constants.h"
#include "Entities/Player.h"
#include "Core/InputManager.h"
#include <iostream>

using namespace Constants;

void WalkState::Enter(Player* player) {
    // 歩きアニメーションの再生開始などをここに書きます
}

void WalkState::Update(Player* player, float dt, const std::vector<SDL_Rect>& colliders) {
    auto& input = InputManager::GetInstance();

    // 左右の加速、速度決定
    player->targetMaxSpeed = MAX_WALK_SPEED;
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        player->SetVelocityX(player->GetVelocityX() + ACCEL_X * dt);
    }
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        player->SetVelocityX(player->GetVelocityX() - ACCEL_X * dt);
    }

    
    // 待機状態（Idle）に戻る条件
    // 移動キーが離されたら（isMovingX が false なら）、待機状態へ
    if (!input.IsKeyDown(SDL_SCANCODE_D) && !input.IsKeyDown(SDL_SCANCODE_RIGHT) &&
        !input.IsKeyDown(SDL_SCANCODE_A) && !input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        // 移動キーが離されたので、待機状態へ
        player->ChangeState(new IdleState());
        return;
    }

    // 歩き状態からダッシュ状態に遷移する条件
    if ((input.IsKeyDown(SDL_SCANCODE_LSHIFT) && input.IsKeyDown(SDL_SCANCODE_D)) || (input.IsKeyDown(SDL_SCANCODE_RSHIFT) && input.IsKeyDown(SDL_SCANCODE_A)) ||
        (input.IsKeyDown(SDL_SCANCODE_LSHIFT) && input.IsKeyDown(SDL_SCANCODE_RIGHT)) || (input.IsKeyDown(SDL_SCANCODE_RSHIFT) && input.IsKeyDown(SDL_SCANCODE_LEFT))) {
        // ダッシュキーが押されたので、ダッシュ状態へ
        player->ChangeState(new DashState());
        // player->ChangeState(new DashState()); // ダッシュ状態のクラスができたらこれを有効にする
        return;
    }

    // ジャンプ状態（Jump）へ遷移する条件
    // ジャンプキーが押された、または崖から落ちた（接地していない）場合
    if (player->IsJumpPressed() || !player->Grounded(colliders)) {
        player->ChangeState(new JumpState());
        return;
    }
}

void WalkState::Exit(Player* player) {
    // 歩き状態から抜けるときの処理
}