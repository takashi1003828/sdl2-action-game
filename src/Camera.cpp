#include "Core/Camera.h"
#include "Entities/Player.h"

Camera::Camera(float startX, float startY, float w, float h){
    x = startX;
    y = startY;
    width = w;
    height = h;
    deadzone.w = 150;
    deadzone.h = h;
    target = nullptr; 
    deadzone.x = (width - deadzone.w) / 2;
    deadzone.y = (height - deadzone.h)/2;
    px = 0;
    py = 0;
}
void Camera::SetTarget(Player* p){
        target = p;
}
void Camera::Update(){
    if(target != nullptr){
            //px,pyにプレイヤーの座標を与える。
            px = target->GetX();
            py = target->GetY();
    }

    float screenPx = px - x;
    float screenPy = py - y;

    if(screenPx < deadzone.x){
        x = px - deadzone.x;
    }
    else if(screenPx > (deadzone.x + deadzone.w)){
        x = px - (deadzone.x + deadzone.w);
    }

}

float Camera::GetX() const{
        return x;
}

float Camera::GetY() const{
        return y;
}
    

    