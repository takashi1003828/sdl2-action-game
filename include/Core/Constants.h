// include/Core/Constants.h
#pragma once

namespace Constants {
    // ウィンドウ設定
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr int TARGET_FPS = 60;
    constexpr unsigned int FRAME_DELAY = 1000 / TARGET_FPS;

    // 物理演算用の定義
    constexpr float PIXEL_PER_METER = 50.0f;
    
    

    // 物理演算用の定数（メートル単位）
    constexpr float GRAVITY_M = 9.8f * 2.0f;
    constexpr float MAX_FALL_SPEED_M = 20.0f;
    constexpr float JUMP_FORCE_M = -12.0f;
    constexpr float MOVE_SPEED_M = 6.0f;
    constexpr float ACCEL_X_M = 50.0f;
    constexpr float ACCEL_A_X_M = 20.0f;
    constexpr float FRICTION_X_M = 100.0f;
    constexpr float MAX_WALK_SPEED_M = 8.0f;
    constexpr float MAX_DASH_SPEED_M = 12.0f;

    // 内部計算用の定数（ピクセル単位に変換）
    constexpr float GRAVITY = GRAVITY_M * PIXEL_PER_METER;
    constexpr float FALL_GRAVITY = GRAVITY * 3.0f;
    constexpr float MAX_FALL_SPEED = MAX_FALL_SPEED_M * PIXEL_PER_METER;
    constexpr float JUMP_FORCE = JUMP_FORCE_M * PIXEL_PER_METER;
    constexpr float MOVE_SPEED = MOVE_SPEED_M * PIXEL_PER_METER;
    constexpr float ACCEL_X = ACCEL_X_M * PIXEL_PER_METER;
    constexpr float ACCEL_A_X = ACCEL_A_X_M * PIXEL_PER_METER;
    constexpr float FRICTION_X = FRICTION_X_M * PIXEL_PER_METER;
    constexpr float MAX_WALK_SPEED = MAX_WALK_SPEED_M * PIXEL_PER_METER;
    constexpr float MAX_DASH_SPEED = MAX_DASH_SPEED_M * PIXEL_PER_METER;

    constexpr float GROUND_Y = 500.0f;
}
