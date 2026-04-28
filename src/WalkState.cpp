#include "Entities/States/WalkState.h"
#include "Entities/States/IdleState.h"
#include "Entities/States/JumpState.h"
#include "Entities/Player.h"

void WalkState::Enter(Player* player) {
    // 歩きアニメーションの再生開始などをここに書きます
}

void WalkState::Update(Player* player, float dt) {
    // 1. 歩行の速度をプレイヤーに適用する
    // （実際には入力方向に応じて + か - の速度を入れます）
    player->SetVelocityX(5.0f); // 仮で右向きの速度をセット

    // 2. 待機状態（Idle）に戻る条件
    // 移動キーが離されたら（isMovingX が false なら）、待機状態へ
    if (!player->IsMovingX()) {
        player->ChangeState(new IdleState());
        return;
    }

    // 3. ジャンプ状態（Jump）へ遷移する条件
    // ジャンプキーが押された、または崖から落ちた（接地していない）場合
    if (player->IsJumpPressed() || !player->IsGrounded()) {
        player->ChangeState(new JumpState());
        return;
    }
}

void WalkState::Exit(Player* player) {
    // 歩き状態から抜けるときの処理
}