#include "Core/CollisionManager.h" // 自分のヘッダーを読み込む（パスは環境に合わせてください）
#include "Physics/Collider.h"         // 実際の計算アルゴリズム（Physics::CheckCollision）

// 関数の「実装（中身）」
bool CollisionManager::CheckPlayerVsMap(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& mapWalls) {
    for (const auto& wall : mapWalls) {
        // collider.h にある CheckCollision を呼び出して判定
        if (Physics::CheckCollision(playerRect, wall)) {
            return true; // 1つでもぶつかっていたら true
        }
    }
    return false; // 全部の壁をチェックして、どれともぶつかっていなければ false
}

void CollisionManager::ResolveMapCollisionX(float& x, float& velocityX, SDL_Rect& playerRect, const std::vector<SDL_Rect>& colliders) {
    
    for(const auto& collider : colliders) {
        // ぶつかっているかチェック
        if(Physics::CheckCollision(playerRect, collider)) {
            
            // 右移動中にぶつかった場合
            if (velocityX > 0) {
                // playerRect.w を使えば、Playerクラスの width をわざわざ引数で渡さなくてもOKです
                x = collider.x - playerRect.w; 
            } 
            // 左移動中にぶつかった場合
            else if (velocityX < 0) {
                x = collider.x + collider.w;
            }
            
            // ぶつかったので速度をゼロにして、Rectの位置も修正
            velocityX = 0.0f;
            playerRect.x = (int)x; 
        }
    }
}

void CollisionManager::ResolveMapCollisionY(float& y, float& velocityY, SDL_Rect& playerRect, bool& isGrounded, const std::vector<SDL_Rect>& colliders) {
    
    // まずフラグをリセット
    isGrounded = false; 

    // 1. めり込み解消ループ
    for(const auto& collider : colliders) {
        if(Physics::CheckCollision(playerRect, collider)) {
            
            if (velocityY > 0) { // 落下して床にぶつかった
                y = collider.y - playerRect.h; 
            } else if (velocityY < 0) { // 上昇して天井にぶつかった
                y = collider.y + collider.h; 
            }
            
            velocityY = 0.0f;
            playerRect.y = (int)y; 
        }
    }

    // 2. 確実な着地判定（足元1ピクセル下のチェック）
    if (velocityY >= 0.0f) {
        // playerRect のデータを使って足元の判定枠を作る
        SDL_Rect groundCheck = { playerRect.x, playerRect.y + 1, playerRect.w, playerRect.h };
        
        for(const auto& collider : colliders) {
            if(Physics::CheckCollision(groundCheck, collider)) {
                isGrounded = true; 
                break; // 1つでも床が見つかればOKなので、無駄な計算を省くために break でループを抜ける
            }
        }
    }
}

bool CollisionManager::CheckPlayerVsGoal(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& goalColliders) {
    for (const auto& goal : goalColliders) {
        if (Physics::CheckCollision(playerRect, goal)) {
            return true; // ゴールにぶつかっていたら true
        }
    }
    return false; // 全部のゴールをチェックして、どれともぶつかっていなければ false
}

bool CollisionManager::CheckPlayerVsEnemy(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& goalColliders) {
    for (const auto& goal : goalColliders) {
        if (Physics::CheckCollision(playerRect, goal)) {
            return true; // 敵にぶつかっていたら true
        }
    }
    return false; // 全部の敵をチェックして、どれともぶつかっていなければ false
}