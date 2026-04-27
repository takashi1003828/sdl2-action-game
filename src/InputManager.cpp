// src/InputManager.cpp
#include "Core/InputManager.h"

// コンストラクタ（初期化）
InputManager::InputManager() : currentKeyStates(nullptr), numKeys(0) {
    // SDLからキーボードの配列のポインタと、キーの総数を取得
    currentKeyStates = SDL_GetKeyboardState(&numKeys);
    // 1フレーム前の状態を保存する配列を初期化
    previousKeyStates.assign(currentKeyStates, currentKeyStates + numKeys);
}

// 毎フレームの更新処理
void InputManager::Update() {
    // 現在のフレームのキー状態を、「1フレーム前の状態」として保存しておく
    // ※この後に行われる SDL_PollEvent によって currentKeyStates の中身は自動的に最新になります
    previousKeyStates.assign(currentKeyStates, currentKeyStates + numKeys);
}

// キーが「押しっぱなし」かどうか（移動などに使用）
bool InputManager::IsKeyDown(SDL_Scancode key) const {
    if (key < 0 || key >= numKeys) return false;
    return currentKeyStates[key] != 0;
}

// 押された瞬間かどうかの判定
bool InputManager::IsKeyPressed(SDL_Scancode key) const {
    if (key < 0 || key >= numKeys) return false;
    // 「今は押されている」かつ「1フレーム前は押されていなかった」なら true
    return currentKeyStates[key] != 0 && previousKeyStates[key] == 0;
}
