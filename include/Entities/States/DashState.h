#pragma once
#include "PlayerState.h"

class Player;

class DashState : public PlayerState {
public:

    //状態に入る時の処理
    void Enter(Player* player) override;

    //マイフレームの更新処理
    void Update(Player* player, float dt, const std::vector<SDL_Rect>& colliders) override;

    //状態から出る時の処理
    void Exit(Player* player) override;


};