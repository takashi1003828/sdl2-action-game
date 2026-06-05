#pragma once
#include <SDL2/SDL.h>

namespace Physics {
    // 2つの短形が重なっているかを判定する静的関数
    inline bool CheckCollision(const SDL_Rect& a,const SDL_Rect& b) {
        //AABBのアルゴリズム
        return (a.x < b.x + b.w &&
                a.x + a.w > b.x &&
                a.y < b.y + b.h &&
                a.y + a.h > b.y);
    }
}