// src/Player.cpp
#include "Entities/Player.h"
#include "Core/Constants.h"
#include "Core/InputManager.h"

using namespace Constants;

// プレイヤーの初期化（コンストラクタ）
Player::Player() {
    x = 400.0f;
    y = 100.0f;
    width = (int)(0.5f * PIXEL_PER_METER);
    height = (int)(1.0f * PIXEL_PER_METER);
    velocityX = 0.0f;
    velocityY = 0.0f;
    isGrounded = false;
    isMovingX = false;
    isJumpPressed = false;
}

//  プレイヤーの終了処理（デスコンストラクタ）
Player::~Player() {}

void Player::Update(float dt) {
    auto& input = InputManager::GetInstance();

    // 1. ジャンプ判定
    isJumpPressed = input.IsKeyDown(SDL_SCANCODE_UP) || input.IsKeyDown(SDL_SCANCODE_W) || input.IsKeyDown(SDL_SCANCODE_SPACE);
    if ((input.IsKeyPressed(SDL_SCANCODE_UP) || input.IsKeyPressed(SDL_SCANCODE_W) || input.IsKeyPressed(SDL_SCANCODE_SPACE)) && isGrounded) {
        velocityY = JUMP_FORCE;
        isGrounded = false;
    }

    // 2. 左右の移動と加速
    float targetMaxSpeed = input.IsKeyDown(SDL_SCANCODE_LSHIFT) ? MAX_DASH_SPEED : MAX_WALK_SPEED;
    isMovingX = false;
    float currentAccel = isGrounded ? ACCEL_X : ACCEL_A_X;
    
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        velocityX += currentAccel * dt;
        isMovingX = true;
    }
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        velocityX -= currentAccel * dt;
        isMovingX = true;
    }

    // 3. 摩擦による減速
    if (!isMovingX) {
        if (velocityX > 0) {
            velocityX -= FRICTION_X * dt;
            if (velocityX < 0) velocityX = 0;
        } else if (velocityX < 0) {
            velocityX += FRICTION_X * dt;
            if (velocityX > 0) velocityX = 0;
        }
    }

    // 4. 速度の制限
    if (velocityX > targetMaxSpeed) velocityX = targetMaxSpeed;
    if (velocityX < -targetMaxSpeed) velocityX = -targetMaxSpeed;

    // 5. 重力の計算
    float currentGravity = (velocityY < 0.0f && !isJumpPressed) ? FALL_GRAVITY : GRAVITY;
    velocityY += currentGravity * dt;
    if (velocityY > MAX_FALL_SPEED) velocityY = MAX_FALL_SPEED;

    // 6. 座標の更新
    x += velocityX * dt;
    y += velocityY * dt;

    // 7. 地面との当たり判定
    if (y + height >= GROUND_Y) {
        y = GROUND_Y - height;
        velocityY = 0.0f;
        isGrounded = true;
    } else {
        isGrounded = false;
    }
}

void Player::Render(SDL_Renderer* renderer) {
    SDL_Rect playerRect = { (int)x, (int)y, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);   //　プレイヤーの色(赤)
    SDL_RenderFillRect(renderer, &playerRect);
}
