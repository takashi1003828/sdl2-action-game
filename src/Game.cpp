// src/Game.cpp
#include "Core/Game.h"
#include "Core/Constants.h"
#include "Core/InputManager.h"
#include "Core/Camera.h"
#include <iostream>
#include "Physics/Collider.h"
#include "Core/CollisionManager.h"
#include "Core/PlayState.h"

using namespace Constants;

// コンストラクタとデストラクタ
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), previousTime(0) {}

Game::~Game() {
}

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

    // ★ 1. TTF（フォントシステム）の初期化
    if (TTF_Init() == -1) {
        std::cout << "TTFの初期化エラー: " << TTF_GetError() << std::endl;
        return false;
    }

    // ★ 2. フォントの読み込み（パスとサイズを指定）
    font = TTF_OpenFont("../assets/fonts/dotto.ttf", 30); 
    if (!font) {
        std::cout << "フォント読み込み失敗: " << TTF_GetError() << std::endl;
    } else {
        uiManager.Initialize(renderer, font);
    }


   

    gamestateManager.ChangeState(new PlayState());

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
    uiManager.Shutdown();
    if(font) TTF_CloseFont(font);
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

    gamestateManager.ProcessInput();
}

//時間のカウントとプレイヤー更新処理
void Game::UpdateGame() {
    // フレームレート制御とデルタタイム計算
    Uint64 frameStart = SDL_GetTicks64();
    float dt = (frameStart - previousTime) / 1000.0f;
    if(dt > 0.05f) dt = 0.05f;

    previousTime = frameStart;

    gamestateManager.Update(dt);
     // フレームレート制御
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }
    
    // }else{
    //     auto& input = InputManager::GetInstance();
    //     if (input.IsKeyDown(SDL_SCANCODE_R)){
    //         // ゲームの状態をリセット
    //         currentState = GameState::PLAYING;
    //         // プレイヤーの位置や状態を初期化する処理を呼び出す（例: player.Reset()）
    //         player = Player(); // プレイヤーオブジェクトを新しく作り直すことでリセット
    //         std::cout << "ゲームをリセットしました！" << std::endl;
    //     }
    // }
    
}

//　描画処理
void Game::GenerateOutput() {
    // // 状態に合わせて背景色（空の色）を変える
    // if (currentState == GameState::PLAYING) {
    //     SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // プレイ中：水色
    // } 
    // else if (currentState == GameState::GAME_OVER) {
    //     SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);     // 死亡：血のような赤色
    // } 
    // else if (currentState == GameState::GAME_CLEAR) {
    //     SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);   // クリア：黄金色
    // }

    
    // bool isBlinking = (SDL_GetTicks() / 500) % 2 == 0;

    // if (isBlinking) {
    //     if (currentState == GameState::GAME_OVER) {
    //         uiManager.DrawGameOver(renderer);
    //     }
    //     else if (currentState == GameState::GAME_CLEAR) {
    //         uiManager.DrawGameClear(renderer);
    //     }
    // }
    gamestateManager.Render(renderer, &uiManager);
    // 描画の更新
    SDL_RenderPresent(renderer);
}
