// src/Player.cpp
#include "Entities/Player.h"
#include "Entities/States/PlayerState.h"
#include "Entities/States/IdleState.h"
#include "Entities/States/WalkState.h"
#include "Entities/States/JumpState.h"
#include "Entities/States/DashState.h"
#include "Entities/States/AirState.h"
#include "Core/Constants.h"
#include "Core/InputManager.h"
#include "Physics/Collider.h"
#include <iostream>


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
    currentState = new IdleState(); // 最初は待機状態からスタート
    currentState->Enter(this); // 初期状態のEnter処理を呼び出す
}

//  プレイヤーの終了処理（デスコンストラクタ）
Player::~Player() {}


void Player::Update(float dt, const std::vector<SDL_Rect>& colliders) {
    auto& input = InputManager::GetInstance();
    

    // ジャンプ判定
    isJumpPressed = input.IsKeyDown(SDL_SCANCODE_UP) || input.IsKeyDown(SDL_SCANCODE_W) || input.IsKeyDown(SDL_SCANCODE_SPACE);

    //　ステート更新
    if (currentState != nullptr) {
        currentState->Update(this, dt, colliders);
    } 

    
    std::cout << typeid(*currentState).name() << std::endl;

    // 摩擦による減速
    Friction(dt);

    
    
    // 速度の制限
    if (velocityX > targetMaxSpeed) velocityX = targetMaxSpeed;
    if (velocityX < -targetMaxSpeed) velocityX = -targetMaxSpeed;

    // 重力の計算
    Gravity(dt);

    // プレイヤーの未来の当たり判定枠を用意する
    playerRect = { (int)x, (int)y, width, height };

    //x軸の移動と当たり判定
    MoveX(dt, colliders);
    

    //y軸の移動と当たり判定
    MoveY(dt, colliders);

}




void Player::MoveX(float dt, const std::vector<SDL_Rect>& colliders){
     x += velocityX * dt;
    playerRect.x = (int)x; //移動後のX座標を枠に反映
    for(const auto& collider : colliders){
        if(Physics::CheckCollision(playerRect, collider)){
            if (velocityX > 0){
                x = collider.x - width; //右側の壁にぶつかった場合、プレイヤーの右端を壁の左端に合わせる
            } else if (velocityX < 0){
                x = collider.x + collider.w; //左側の壁にぶつかった場合、プレイヤーの左端を壁の右端に合わせる
            }
            //ぶつかった場合のめり込み解消処理
            velocityX = 0.0f;
            playerRect.x = (int)x; //位置修正後のX座標を枠に反映
        }
    }
}

void Player::MoveY(float dt, const std::vector<SDL_Rect>& colliders){
    y += velocityY * dt;
    playerRect.y = (int)y; //移動後のY座標を枠に反映
    isGrounded = false; //地面にいるかのフラグをリセット
    for(const auto& collider : colliders){
        if(Physics::CheckCollision(playerRect, collider)){
            if (velocityY > 0){
                y = collider.y - height; //地面に落ちる場合、プレイヤーの下端を地面の上端に合わせる
                isGrounded = true; //地面にいるフラグを立てる
            } else if (velocityY < 0){
                y = collider.y + collider.h; //天井にぶつかる場合、プレイヤーの上端を天井の下端に合わせる
            }
            //ぶつかった場合のめり込み解消処理
            velocityY = 0.0f;
            playerRect.y = (int)y; //位置修正後のY座標を枠に反映
        }
    }
    // Y軸の判定が終わった後、足元1ピクセル下を調べて確実に床があるかをチェック
    // 小数点の切り捨てによる「ジャンプ不発」や「ガタつき」を防ぐ堅牢な手法
    isGrounded = false;
    if (velocityY >= 0.0f) {
        SDL_Rect groundCheck = { (int)x, (int)y + 1, width, height };
        for(const auto& collider : colliders){
            if(Physics::CheckCollision(groundCheck, collider)){
                isGrounded = true; // 足元に地面あり
            }
        }
    }
}

bool Player::Grounded(const std::vector<SDL_Rect>& colliders){
    // --- 確実な着地判定（Raycast方式） ---
    // Y軸の判定が終わった後、足元1ピクセル下を調べて確実に床があるかをチェック
    // 小数点の切り捨てによる「ジャンプ不発」や「ガタつき」を防ぐ堅牢な手法
    isGrounded = false;
    if (velocityY >= 0.0f) {
        SDL_Rect groundCheck = { (int)x, (int)y + 1, width, height };
        for(const auto& collider : colliders){
            if(Physics::CheckCollision(groundCheck, collider)){
                return true; // 足元に地面あり
            }
        }
    }
    return false; //足元に地面なし
}

void Player::Gravity(float dt) {
    float currentGravity = (velocityY < 0.0f && !isJumpPressed) ? FALL_GRAVITY : GRAVITY;
    velocityY += currentGravity * dt;
    if (velocityY > MAX_FALL_SPEED) velocityY = MAX_FALL_SPEED;
}

void Player::Friction(float dt){
    //移動キーが押されていないときに摩擦を適用して減速する
        auto& input = InputManager::GetInstance();
    if (!input.IsKeyDown(SDL_SCANCODE_D) && !input.IsKeyDown(SDL_SCANCODE_RIGHT) &&
        !input.IsKeyDown(SDL_SCANCODE_A) && !input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        if (velocityX > 0) {
            velocityX -= FRICTION_X * dt;
            if (velocityX < 0) velocityX = 0;
        } else if (velocityX < 0) {
            velocityX += FRICTION_X * dt;
            if (velocityX > 0) velocityX = 0;
        }
    }

}

void Player::ChangeState(PlayerState* newState) {
    // 1. もし現在何かの状態に入っていれば、終わりの処理(Exit)を呼んでから消す
    if (currentState != nullptr) {
        currentState->Exit(this); // 古い状態の終了処理
        delete currentState;      // 古い状態のメモリを解放！（重要）
    }

    // 2. 新しい状態をセットする
    currentState = newState;

    // 3. 新しい状態の始まりの処理(Enter)を呼ぶ
    if (currentState != nullptr) {
        currentState->Enter(this);
    }
}

void Player::Render(SDL_Renderer* renderer , int camX, int camY) {
    SDL_Rect playerRect = { (int)x - camX, (int)y - camY, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);   //　プレイヤーの色(赤)
    SDL_RenderFillRect(renderer, &playerRect);
}
