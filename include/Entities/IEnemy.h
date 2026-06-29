#pragma once

class IEnemy {
public:
    // 仮想デストラクタ（継承先で正しく破棄されるために必須）
    virtual ~IEnemy() = default;

    // 座標の取得（当たり判定などで使用）
    virtual float GetX() const = 0;
    virtual float GetY() const = 0;

    // 毎フレーム呼ばれる更新処理
    virtual void Update() = 0;

    // 描画処理
    virtual void Draw() = 0;

    // プレイヤーと当たった時の処理
    virtual void OnHitPlayer() = 0;
};