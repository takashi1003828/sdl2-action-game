#include "Entities/States/IdleState.h"
#include "Entities/States/WalkState.h"
#include "Entities/States/JumpState.h"
#include "Entities/Player.h"
#include "Core/Constants.h"

void JumpState::Enter(Player* player) {
    // ジャンプ状態に入った瞬間、垂直方向の速度を設定する
    player->SetVelocityY(-12.0f); // 仮で上向きの速度をセット
}

void JumpState::Update(Player* player, float dt) {
    // ジャンプ中の物理演算などをここに書きます
}

void JumpState::Exit(Player* player) {
    // ジャンプ状態から抜けるときの処理
}
    
    // （将来的に）待機アニメーションの再生指示などもここに書きます
}

void IdleState::Update(Player* player, float dt) {
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