// include/Core/Game.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Entities/Player.h"

class Game {
public:
    Game();
    ~Game();

    // ゲームの初期化（ウィンドウ作成など）
    bool Initialize(const char* title, int width, int height);
    // メインループの実行
    void RunLoop();
    // 終了処理
    void Shutdown();

private:
    // ループ内で毎フレーム呼ばれる3つの主要関数
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    Uint64 previousTime;

    Player player;

    //ステージ上の当たり判定を持つブロックのリスト
    std::vector<SDL_Rect> colliders;
};
