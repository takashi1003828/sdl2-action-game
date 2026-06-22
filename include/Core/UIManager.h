// UIManager.h
#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

class UIManager {
private:
    SDL_Texture* textGameOver;
    SDL_Texture* textGameClear;
    SDL_Rect gameOverRect;
    SDL_Rect gameClearRect;

public:
    UIManager();
    ~UIManager();

    // 初期化（Gameから renderer と font をもらう！）
    bool Initialize(SDL_Renderer* renderer, TTF_Font* font);

    // 解放処理
    void Shutdown();

    // 描画用の関数
    void DrawGameOver(SDL_Renderer* renderer);
    void DrawGameClear(SDL_Renderer* renderer);
};