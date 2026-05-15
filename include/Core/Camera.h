#pragma once
#include <SDL2/SDL.h>

class Player;

class Camera{
public:
    Camera(float startX, float startY, float w, float h);
    void SetTarget(Player* p);
    void Update();
    float GetX() const;
    float GetY() const;
    
private:
    //カメラ左上の座標
    float x = 0.0f, y = 0.0f;
    //プレイヤーの座標
    float px = 0.0f, py = 0.0f;
    //カメラの解像度
    float width = 0.0f, height = 0.0f;
    //誰を追従するか
    Player* target = nullptr;
    //カメラの遊び
    SDL_Rect deadzone = {0, 0, 0, 0};
    //カメラ移動の最大、最小値
    float mapMinX = 0.0f, mapMaxX = 0.0f, mapMinY = 0.0f, mapMaxY = 0.0f;

};