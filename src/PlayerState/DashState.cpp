#include "Entities/States/DashState.h"
#include "Entities/States/IdleState.h"
#include "Entities/States/JumpState.h"
#include "Entities/States/WalkState.h"
#include "Entities/States/AirState.h"
#include "Core/Constants.h"
#include "Entities/Player.h"
#include "Core/InputManager.h"

using namespace Constants;

void DashState::Enter(Player* player) {
    // ダッシュアニメーションの再生開始などをここに書きます
}

void DashState::Update(Player* player, float dt, const std::vector<SDL_Rect>& colliders) {
    auto& input = InputManager::GetInstance();

    // 2. 左右の加速、速度決定
    player->targetMaxSpeed = MAX_DASH_SPEED;
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        player->SetVelocityX(player->GetVelocityX() + ACCEL_X * dt);
    }
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        player->SetVelocityX(player->GetVelocityX() - ACCEL_X * dt);
    }

    // 待機状態に戻る条件
    // 移動キーが離されたら、待機状態へ
    if (!input.IsKeyDown(SDL_SCANCODE_D) && !input.IsKeyDown(SDL_SCANCODE_RIGHT) &&
        !input.IsKeyDown(SDL_SCANCODE_A) && !input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        // 移動キーが離されたので、待機状態へ
        player->ChangeState(new IdleState());
        return;
    }

    // ダッシュ状態から歩き状態に遷移する条件
    if (!input.IsKeyDown(SDL_SCANCODE_LSHIFT)) {
        // ダッシュキーが離されたので歩きに遷移
        player->ChangeState(new WalkState());
        return;
    }

    // ジャンプ状態（Jump）へ遷移する条件
    // ジャンプキーが押されている、かつ地上にいるとき
    if (player->IsJumpPressed() && player->Grounded(colliders)) {
        player->ChangeState(new JumpState());
        return;
    }

    //もしジャンプボタンが離されていて、かつ空中にいるなら
    if (!input.IsKeyDown(SDL_SCANCODE_SPACE) && !player->Grounded(colliders)) {
        player->ChangeState(new AirState());
        return;
    }
}

void DashState::Exit(Player* player) {
    // ダッシュ状態から抜けるときの処理
}