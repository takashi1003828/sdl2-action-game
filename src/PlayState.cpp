#include "Core/PlayState.h"
#include "Core/GameStateManager.h"
#include "Core/CollisionManager.h"
#include "Core/GameOverState.h"   // 忘れずにインクルード！
#include "Core/GameClearState.h"  // 忘れずにインクルード！
#include "Core/UIManager.h"
#include <iostream>

// コンストラクタ（public: は不要。PlayState:: を頭につける）
PlayState::PlayState() : myMap(32), camera(nullptr) {
    camera = new Camera(0.0f, 0.0f, 800.0f, 600.0f);
    camera->SetTarget(&player);
}

// デストラクタ
PlayState::~PlayState() {
    delete camera;
}

void PlayState::OnEnter() {
    if (!myMap.LoadFromCSV("../assets/maps/stage1.csv")) {
        std::cout << "マップの読み込みに失敗しました！" << std::endl;
    } else {
        std::cout << "[Init] Map loaded! Wall Count: " << myMap.GetWallColliders().size() << std::endl;
    }   
}

void PlayState::OnExit() {
}

void PlayState::ProcessInput() {
}

void PlayState::Update(float dt, GameStateManager* gsm) {
    // ゲームオーバーの判定
    if(player.GetY() > 600.0f){
        std::cout << "もう終わりだよ...奈落に落ちた..." << std::endl;
        gsm->ChangeState(new GameOverState());
        return; // ★超重要：切り替えたら即座にこの関数を抜ける！
    }
    
    // ゲームクリアの判定
    if(CollisionManager::CheckPlayerVsGoal(player.GetRect(), myMap.GetGoalColliders())){
        std::cout << "ステージクリア！おめでとう！" << std::endl;
        gsm->ChangeState(new GameClearState());
        return; // ★超重要：切り替えたら即座にこの関数を抜ける！
    }

    // 「mapWalls」を取得（誕生）させる！
    const std::vector<SDL_Rect>& mapWalls = myMap.GetWallColliders();
    
    // Playerの更新処理
    player.Update(dt, mapWalls);
    
    // カメラの更新も忘れずに！
    camera->Update(); 
}

// Renderの中身が空だと画面が真っ暗になるので、以前の描画コードを入れています
void PlayState::Render(SDL_Renderer* renderer, UIManager* uiManager) {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // プレイ中：水色
    SDL_RenderClear(renderer);

    int camX = (int)camera->GetX();
    int camY = (int)camera->GetY();


    // 地形の描画処理
    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255); //地形の色(緑)

    const std::vector<SDL_Rect>& colliders = myMap.GetWallColliders();

    myMap.Render(renderer, camX, camY);
    // Playerの描画処理
    player.Render(renderer, camX, camY);

}