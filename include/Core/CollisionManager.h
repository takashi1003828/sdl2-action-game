#pragma once
#include <vector>
#include <SDL2/SDL.h>

class CollisionManager {
public:
    // プレイヤーとマップの壁の当たり判定をチェックする関数の「宣言」
    static bool CheckPlayerVsMap(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& mapWalls);

    static void ResolveMapCollisionX(float& x, float& velocityX, SDL_Rect& playerRect, const std::vector<SDL_Rect>& colliders);

    static void ResolveMapCollisionY(float& y, float& velocityY, SDL_Rect& playerRect, bool& isGrounded, const std::vector<SDL_Rect>& colliders);
    
    static bool CheckPlayerVsGoal(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& goalColliders) ;

    static bool CheckPlayerVsEnemy(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& goalColliders) ;

};