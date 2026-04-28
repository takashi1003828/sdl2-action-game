// src/Player.cpp
#include "Entities/Player.h"
#include "Entities/States/PlayerState.h"
#include "Core/Constants.h"
#include "Core/InputManager.h"
#include "Physics/Collider.h"

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

void Player::Update(float dt, const std::vector<SDL_Rect>& colliders) {
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

 

    // プレイヤーの未来の当たり判定枠を用意する
    SDL_Rect playerRect = { (int)x, (int)y, width, height };
    //
    //x軸の移動と当たり判定
    //
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

    //
    //y軸の移動と当たり判定
    //
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
    // --- 確実な着地判定（Raycast方式） ---
    // Y軸の判定が終わった後、足元1ピクセル下を調べて確実に床があるかをチェックします。
    // 小数点の切り捨てによる「ジャンプ不発」や「ガタつき」を防ぐ堅牢な手法です。
    isGrounded = false;
    if (velocityY >= 0.0f) {
        SDL_Rect groundCheck = { (int)x, (int)y + 1, width, height };
        for(const auto& collider : colliders){
            if(Physics::CheckCollision(groundCheck, collider)){
                isGrounded = true;
                break;
            }
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

void Player::Render(SDL_Renderer* renderer) {
    SDL_Rect playerRect = { (int)x, (int)y, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);   //　プレイヤーの色(赤)
    SDL_RenderFillRect(renderer, &playerRect);
}
