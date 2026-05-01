#include "Entities/States/IdleState.h"
#include "Entities/States/WalkState.h"
#include "Entities/States/JumpState.h"
#include "Entities/States/AirState.h"
#include "Entities/States/DashState.h"
#include "Core/InputManager.h"
#include "Entities/Player.h"
#include "Core/Constants.h"

using namespace Constants;

void JumpState::Enter(Player* player) {
    player->SetVelocityY(JUMP_FORCE);
}

void JumpState::Update(Player* player, float dt, const std::vector<SDL_Rect>& colliders) {
    auto& input = InputManager::GetInstance();
    // 左右の加速、速度決定
    player->targetMaxSpeed = MAX_WALK_SPEED;
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        player->SetVelocityX(player->GetVelocityX() + ACCEL_A_X * dt);
    }
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        player->SetVelocityX(player->GetVelocityX() - ACCEL_A_X * dt);
    }
    
    //もしジャンプキーが押されているかつ地面にいるなら
    if (player->IsJumpPressed() && player->Grounded(colliders)) {
        player->ChangeState(new JumpState());
        return;
    }

    // もしジャンプボタンが離されているかつ空中にいるなら
    if (!player->IsJumpPressed() && !player->Grounded(colliders)) {
        player->ChangeState(new AirState());
        return;
    }
}

void JumpState::Exit(Player* player) {
    // ジャンプ状態から抜けるときの処理
}