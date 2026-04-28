#pragma once

class Player;

class PlayerState {
public:
    virtual ~PlayerState(){}

    //状態に入る時の処理
    virtual void Enter(Player* player) = 0;

    //マイフレームの更新処理
    virtual void Update(Player* player, float dt) = 0;

    //状態から出る時の処理
    virtual void Exit(Player* player) = 0;



};