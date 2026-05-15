// include/Entities/Player.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>

class PlayerState; // 前方宣言（PlayerStateクラスの完全な定義はPlayer.cppで行います）

class Player {
public:
    Player();
    ~Player();

    // 毎フレームの計算（入力・物理演算）を行う
    void Update(float dt, const std::vector<SDL_Rect>& colliders);
    
    // 描画を行う
    void Render(SDL_Renderer* renderer, int camX, int camY);

    // --- ステートパターン用の追加 ---
    // 状態を切り替える関数
    void ChangeState(PlayerState* newState);

    // 状態クラスからプレイヤーの変数を操作するためのゲッター/セッター
    float GetX() const { return x; }
    float GetY() const { return y; }

    float targetMaxSpeed; // 状態クラスがプレイヤーの最大速度を指示するための変数
    float GetVelocityX() const { return velocityX; }
    float GetVelocityY() const { return velocityY; }
    void SetVelocityX(float vx) { velocityX = vx; }
    void SetVelocityY(float vy) { velocityY = vy; }

    bool IsGrounded() const { return isGrounded; }
    bool IsMovingX() const { return isMovingX; }
    bool IsJumpPressed() const { return isJumpPressed; }
    bool Grounded(const std::vector<SDL_Rect>& colliders);
private:
    // プレイヤーの状態データ
    PlayerState* currentState; // 現在の状態を指すポインタ

    float x;
    float y;
    float velocityX;
    float velocityY;
    int width;
    int height;
    bool isGrounded;
    bool isMovingX;
    bool isJumpPressed;
    SDL_Rect playerRect; // プレイヤーの当たり判定用の矩形
    void MoveX(float dt, const std::vector<SDL_Rect>& colliders);
    void MoveY(float dt, const std::vector<SDL_Rect>& colliders);
    void Gravity(float dt);
    void Friction(float dt);
};
