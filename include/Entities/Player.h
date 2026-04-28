// include/Entities/Player.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Player {
public:
    Player();
    ~Player();

    // 毎フレームの計算（入力・物理演算）を行う
    void Update(float dt, const std::vector<SDL_Rect>& colliders);
    
    // 描画を行う
    void Render(SDL_Renderer* renderer);

private:
    // プレイヤーの状態データ
    float x;
    float y;
    float velocityX;
    float velocityY;
    int width;
    int height;
    bool isGrounded;
    bool isMovingX;
    bool isJumpPressed;
};
