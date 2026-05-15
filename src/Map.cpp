#include "Core/Map.h"
#include <fstream>
#include <sstream>
#include <iostream> // エラー出力用

// コンストラクタ
Map::Map(int tileSize) {
    this->tileSize = tileSize;
}

// CSV読み込み関数
bool Map::LoadFromCSV(const std::string& filePath) {
    // 1. ファイルを開く
    std::ifstream file(filePath);

    // ファイルが開けなかった場合のエラーハンドリング
    if (!file.is_open()) {
        std::cerr << "エラー: マップファイルが開けません - " << filePath << std::endl;
        return false; 
    }

    // 以前のデータが残っていたらクリアしておく（再読み込み対応）
    mapData.clear();

    std::string line;
    
    // 2. ファイルから1行ずつ読み込む (lineに文字列が入る)
    while (std::getline(file, line)) {
        
        std::vector<int> rowData;      // 1行分のデータを格納する配列
        std::stringstream ss(line);    // 読み込んだ1行をストリーム（分割機）にかける
        std::string cell;
        
        // 3. カンマ ',' を区切り文字として、1セルずつ取り出す
        while (std::getline(ss, cell, ',')) {
            // 取り出した文字列(cell)を、整数(int)に変換して配列に追加する
            rowData.push_back(std::stoi(cell));
        }

        // 4. 1行分のデータ(rowData)が完成したら、マップデータ本体に追加する
        mapData.push_back(rowData);
    }

    // 使い終わったファイルは閉じる
    file.close();
    return true;
}

// -----------------------------------------------------------------
// Render関数は前回お見せした二重ループの処理をここに書きます
// -----------------------------------------------------------------
void Map::Render(SDL_Renderer* renderer, int camX, int camY) {
    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255); // 緑色

    for (int row = 0; row < mapData.size(); ++row) {
        for (int col = 0; col < mapData[row].size(); ++col) {
            if (mapData[row][col] == 1) { // 1なら床
                int worldX = col * tileSize;
                int worldY = row * tileSize;

                // カメラの座標を引いて描画位置を決定
                SDL_Rect blockRect = { worldX - camX, worldY - camY, tileSize, tileSize };
                SDL_RenderFillRect(renderer, &blockRect);
            }
        }
    }
}