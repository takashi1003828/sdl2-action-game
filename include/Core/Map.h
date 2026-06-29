#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Camera;

class Map {
public:
    // コンストラクタで「1マスのサイズ（例: 32pxや64px）」を決める
    Map(int tileSize);
    
    // CSVファイルを読み込んで mapData に格納する関数
    bool LoadFromCSV(const std::string& filePath);
    
    // 描画処理（カメラの座標を受け取る！）
    void Render(SDL_Renderer* renderer, int camX, int camY);

    // 壁の当たり判定を取得する
    const std::vector<SDL_Rect>& GetWallColliders() const { return wallColliders; }
    // ゴールの当たり判定を取得する
    const std::vector<SDL_Rect>& GetGoalColliders() const { return goalColliders; }

    const std::vector<SDL_Rect>& GetEnemyColliders() const { return enemyColliders; }
private:
    int tileSize;
    // マップデータ本体（縦×横の2次元配列）
    std::vector<std::vector<int>> mapData; 

    std::vector<SDL_Rect> GetSurround(SDL_Rect playerRect);
    std::vector<SDL_Rect> wallColliders;
    std::vector<SDL_Rect>goalColliders;
    std::vector<SDL_Rect>enemyColliders;
};