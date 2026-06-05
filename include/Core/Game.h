// include/Core/Game.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Entities/Player.h"
#include "Core/Map.h"

class Camera;

enum class GameState {
    PLAYING,
    GAME_CLEAR,
    GAME_OVER,
};

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
    GameState currentState = GameState::PLAYING; // ゲームの状態を管理する変数


    SDL_Window* window;
    SDL_Renderer* renderer;
    Camera* camera;
    bool isRunning;
    Uint64 previousTime;

    Player player;
    Map myMap;

    //ステージ上の当たり判定を持つブロックのリスト
    std::vector<SDL_Rect> colliders;
};
