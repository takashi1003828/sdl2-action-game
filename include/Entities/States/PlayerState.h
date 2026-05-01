#pragma once
#include <vector>  // ← これがないと「std::vector」が使えない
#include <SDL.h>   // ← これがないと「SDL_Rect」が使えない
class Player;
class Collider;
class PlayerState {
public:
    virtual ~PlayerState(){}

    //状態に入る時の処理
    virtual void Enter(Player* player) = 0;

    //マイフレームの更新処理
    virtual void Update(Player* player, float dt, const std::vector<SDL_Rect>& colliders) = 0;

    //状態から出る時の処理
    virtual void Exit(Player* player) = 0;



};