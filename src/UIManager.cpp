// UIManager.cpp
#include "Core/UIManager.h"
#include <iostream>

UIManager::UIManager() : textGameOver(nullptr), textGameClear(nullptr) {}

UIManager::~UIManager() {
    Shutdown();
}

bool UIManager::Initialize(SDL_Renderer* renderer, TTF_Font* font) {
    if (!renderer || !font) return false;

    SDL_Color white = {255, 255, 255, 255};

    TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_CENTER);
    
    // GAME OVER のテクスチャ作成
    SDL_Surface* overSurface = TTF_RenderUTF8_Blended_Wrapped(font, "GAME OVER \n -Press [R] to Restart-", white, 800);
    textGameOver = SDL_CreateTextureFromSurface(renderer, overSurface);
    gameOverRect = { (800 - overSurface->w) / 2, 200, overSurface->w, overSurface->h };
    SDL_FreeSurface(overSurface);

    // GAME CLEAR のテクスチャ作成
    SDL_Surface* clearSurface = TTF_RenderUTF8_Blended_Wrapped(font, "GAME CLEAR!! \n -Press [R] to Restart-", white, 800);
    textGameClear = SDL_CreateTextureFromSurface(renderer, clearSurface);
    gameClearRect = { (800 - clearSurface->w) / 2, 200, clearSurface->w, clearSurface->h };
    SDL_FreeSurface(clearSurface);

    return true;
}

void UIManager::Shutdown() {
    // 自分が作ったテクスチャは自分で責任を持って破棄する
    if (textGameOver) {
        SDL_DestroyTexture(textGameOver);
        textGameOver = nullptr;
    }
    if (textGameClear) {
        SDL_DestroyTexture(textGameClear);
        textGameClear = nullptr;
    }
}

// ゲームオーバー文字の描画を代行する
void UIManager::DrawGameOver(SDL_Renderer* renderer) {
    if (textGameOver) {
        SDL_RenderCopy(renderer, textGameOver, NULL, &gameOverRect);
    }
}

// ゲームクリア文字の描画を代行する
void UIManager::DrawGameClear(SDL_Renderer* renderer) {
    if (textGameClear) {
        SDL_RenderCopy(renderer, textGameClear, NULL, &gameClearRect);
    }
}