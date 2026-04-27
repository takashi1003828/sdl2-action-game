// src/Game.cpp
#include "Core/Game.h"
#include "Core/Constants.h"
#include "Core/InputManager.h"
#include <iostream>

using namespace Constants;

// --- 第4回でスッキリしたコンストラクタ ---
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), previousTime(0) {}

Game::~Game() {}

//SDLやウィンドウ、レンダラーの初期化
bool Game::Initialize(const char* title, int width, int height) {
    // SDLの初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDLの初期化に失敗しました: " << SDL_GetError() << std::endl;
        return false;
    }

    //　ウィンドウとレンダラー作成
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "ウィンドウ作成失敗: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "レンダラー作成失敗: " << SDL_GetError() << std::endl;
        return false;
    }

    // ループの準備
    isRunning = true;
    previousTime = SDL_GetTicks64();
    return true;
}

//　メインループ
void Game::RunLoop() {
    while (isRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

//　SDLの終了処理
void Game::Shutdown() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

// xを押したら閉じる処理と、InputManagerの更新処理
void Game::ProcessInput() {
    // InputManagerの更新処理
    InputManager::GetInstance().Update();

    //　xボタンで閉じる処理
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

//時間のカウントとプレイヤー更新処理
void Game::UpdateGame() {
    // フレームレート制御とデルタタイム計算
    Uint64 frameStart = SDL_GetTicks64();
    float dt = (frameStart - previousTime) / 1000.0f;
    if(dt > 0.05f) dt = 0.05f;
    previousTime = frameStart;

    // Playerの更新処理
    player.Update(dt);

    // フレームレート制御
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }
}

//　描画処理
void Game::GenerateOutput() {
    // 背景を塗りつぶし
    SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
    SDL_RenderClear(renderer);

    // Playerの描画処理
    player.Render(renderer);

    // 描画の更新
    SDL_RenderPresent(renderer);
}
