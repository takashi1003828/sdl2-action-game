// --- Dmman.h ---
#pragma once
#include "Entities/IEnemy.h"

class Dmman : public IEnemy {
private:
    float m_x;
    float m_y;

public:
    Dmman(float startX, float startY);

    float GetX() const override;
    float GetY() const override;
    void Update() override;
    void Draw() override;
    void OnHitPlayer() override;
};