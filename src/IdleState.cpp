#include "Entities/States/IdleState.h"
#include "Entities/States/WalkState.h" // 遷移先のクラスを知る必要があるためインクルードします
#include "Entities/Player.h"
#include "Core/Constants.h"

void IdleState::Enter(Player* player) {
    
    // （将来的に）待機アニメーションの再生指示などもここに書きます
}

void IdleState::Update(Player* player, float dt) {

    // 1. 現在のX速度を取得する
    float currentVx = player->GetVelocityX();

    // 3. 摩擦による減速
    if (currentVx > 0) {
        currentVx -= FRICTION_X * dt;
        if (currentVx < 0) currentVx = 0;
    } else if (currentVx < 0) {
        currentVx += FRICTION_X * dt;
            if (currentVx > 0) currentVx = 0;
    }

    player->SetVelocityX(currentVx);
    



    // プレイヤーが左右の移動キーを押しているかチェック
    if (player->IsMovingX()) {
        // プレイヤーに対して「歩き状態」への変更を指示する！
        player->ChangeState(new WalkState());
        return; // 状態が変わったので、このフレームのUpdate処理はここで終了
    }
    
    // もしジャンプボタンが押されていて、かつ地面にいるなら
    if (player->IsJumpPressed() && player->IsGrounded()) {
        player->ChangeState(new JumpState());
        return;
    }
}

void IdleState::Exit(Player* player) {
    // 待機状態から抜けるときの処理（特になければ空でOKです）
}