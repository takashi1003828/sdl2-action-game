#include "Entities/States/WalkState.h"
#include "Entities/States/IdleState.h"
#include "Entities/States/JumpState.h"
#include "Entities/States/DashState.h"
#include "Entities/States/AirState.h"
#include "Core/Constants.h"
#include "Entities/Player.h"
#include "Core/InputManager.h"


using namespace Constants;

void AirState::Enter(Player* player) {
}


void AirState::Update(Player* player, float dt, const std::vector<SDL_Rect>& colliders) {
    auto& input = InputManager::GetInstance();

    // 2. 左右の加速、速度決定
    player->targetMaxSpeed = MAX_WALK_SPEED;
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        player->SetVelocityX(player->GetVelocityX() + ACCEL_A_X * dt);
    }
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        player->SetVelocityX(player->GetVelocityX() - ACCEL_A_X * dt);
    }


    // 待機状態（Idle）に戻る条件
    // 地上に着地したら待機状態へ
    if (player->Grounded(colliders)) {
        player->ChangeState(new IdleState());
        return;
    }
}

void AirState::Exit(Player* player) {
    // 空中状態から抜けるときの処理
}