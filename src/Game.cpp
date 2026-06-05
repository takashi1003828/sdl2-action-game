// src/Game.cpp
#include "Core/Game.h"
#include "Core/Constants.h"
#include "Core/InputManager.h"
#include "Core/Camera.h"
#include <iostream>
#include "Physics/Collider.h"
#include "Core/CollisionManager.h"


using namespace Constants;

// コンストラクタとデストラクタ
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), previousTime(0),myMap(32) {}

Game::~Game() {
    delete camera;
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
    // ※ assets/fonts フォルダを作り、適当なフリーフォント（.ttf）を入れてください！
    font = TTF_OpenFont("../assets/fonts/dotto.ttf", 30); 
    if (!font) {
        std::cout << "フォント読み込み失敗: " << TTF_GetError() << std::endl;
    } else {
        CreateTextTextures(); // テクスチャ生成関数を呼ぶ
    }

    camera = new Camera(0.0f, 0.0f, 800.0f, 600.0f);
    camera->SetTarget(&player);

    myMap = Map(32); 

    // プロジェクトルートからの相対パスを指定
    if (!myMap.LoadFromCSV("../assets/maps/stage1.csv")) {
        std::cout << "マップの読み込みに失敗しました！パスを確認してください。" << std::endl;
        // 読み込み失敗時の処理（ゲームを終了するなど）
    }

    // ★ デバッグ出力1：読み込み直後の壁の数を確認！
    std::cout << "[Init] Map loaded! Wall Count: " << myMap.GetWallColliders().size() << std::endl;


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
        camera->Update();
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
    if(currentState == GameState::PLAYING){
        // フレームレート制御とデルタタイム計算
        Uint64 frameStart = SDL_GetTicks64();
        float dt = (frameStart - previousTime) / 1000.0f;
        if(dt > 0.05f) dt = 0.05f;
        previousTime = frameStart;

         //ゲームオーバーの判定
        if(player.GetY() > 600.0f){
            currentState = GameState::GAME_OVER;
            std::cout << "もう終わりだよ...奈落に落ちた..." << std::endl;
        }
        //ゲームクリアの判定
        if(CollisionManager::CheckPlayerVsGoal(player.GetRect(), myMap.GetGoalColliders())){
            currentState = GameState::GAME_CLEAR;
            std::cout << "ステージクリア！おめでとう！" << std::endl;
        }

        // 「mapWalls」を取得（誕生）させる！
        const std::vector<SDL_Rect>& mapWalls = myMap.GetWallColliders();


        // Playerの更新処理
        player.Update(dt, mapWalls);

        const std::vector<SDL_Rect>& goalColliders = myMap.GetGoalColliders();

        // フレームレート制御
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }else{
        auto& input = InputManager::GetInstance();
        if (input.IsKeyDown(SDL_SCANCODE_R)){
            // ゲームの状態をリセット
            currentState = GameState::PLAYING;
            // プレイヤーの位置や状態を初期化する処理を呼び出す（例: player.Reset()）
            player = Player(); // プレイヤーオブジェクトを新しく作り直すことでリセット
            std::cout << "ゲームをリセットしました！" << std::endl;
        }
    }
    
}

void Game::CreateTextTextures() {
    SDL_Color white = {255, 255, 255, 255}; // 文字色（白）

    TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_CENTER);
    // GAME OVER のテクスチャ作成
    SDL_Surface* overSurface = TTF_RenderUTF8_Blended_Wrapped(font, "GAME OVER \n -Press [R] to Restart-", white, 800);
    textGameOver = SDL_CreateTextureFromSurface(renderer, overSurface);
    gameOverRect = { (800 - overSurface->w) / 2, 200, overSurface->w, overSurface->h }; // 画面中央に配置（画面幅800の場合）
    SDL_FreeSurface(overSurface);

    // GAME CLEAR のテクスチャ作成
    SDL_Surface* clearSurface = TTF_RenderUTF8_Blended_Wrapped(font, "GAME CLEAR!! \n -Press [R] to Restart-", white, 800);
    textGameClear = SDL_CreateTextureFromSurface(renderer, clearSurface);
    gameClearRect = { (800 - clearSurface->w) / 2, 200, clearSurface->w, clearSurface->h };
    SDL_FreeSurface(clearSurface);
}

//　描画処理
void Game::GenerateOutput() {
    // 状態に合わせて背景色（空の色）を変える
    if (currentState == GameState::PLAYING) {
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // プレイ中：水色
    } 
    else if (currentState == GameState::GAME_OVER) {
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);     // 死亡：血のような赤色
    } 
    else if (currentState == GameState::GAME_CLEAR) {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);   // クリア：黄金色
    }

    // 画面を塗りつぶす
    SDL_RenderClear(renderer);

    int camX = (int)camera->GetX();
    int camY = (int)camera->GetY();


    // 地形の描画処理
    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255); //地形の色(緑)
    for (const auto& collider : colliders){
        SDL_Rect renderRect = collider;

        renderRect.x -= camX;
        renderRect.y -= camY;
        SDL_RenderFillRect(renderer, &renderRect);
    }

    myMap.Render(renderer, camX, camY);
    // Playerの描画処理
    player.Render(renderer, camX, camY);

    bool isBlinking = (SDL_GetTicks() / 500) % 2 == 0;

    if (isBlinking) {
        if (currentState == GameState::GAME_OVER && textGameOver != nullptr) {
            SDL_RenderCopy(renderer, textGameOver, NULL, &gameOverRect);
        }
        else if (currentState == GameState::GAME_CLEAR && textGameClear != nullptr) {
            SDL_RenderCopy(renderer, textGameClear, NULL, &gameClearRect);
        }
    }

    // 描画の更新
    SDL_RenderPresent(renderer);
}
