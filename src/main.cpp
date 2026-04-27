// src/main.cpp
#include "Core/Game.h"
#include "Core/Constants.h"

int main(int argc, char* argv[]) {
    Game game;

    // ゲームの初期化
    if (game.Initialize("actionman - Prototype", Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT)) {
        // メインループの開始
        game.RunLoop();
    }

    // 終了処理
    game.Shutdown();

    return 0;
}
