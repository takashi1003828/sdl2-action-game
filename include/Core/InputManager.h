// include/Core/InputManager.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>

class InputManager {
public:
    // どこからでもこのクラスのインスタンスを取得できるようにする
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }

    // 毎フレーム呼ぶ更新処理
    void Update();

    // キーが「押しっぱなし」かどうか（移動などに使用）
    bool IsKeyDown(SDL_Scancode key) const;

    // キーが「押された瞬間」かどうか（ジャンプや攻撃に使用）
    bool IsKeyPressed(SDL_Scancode key) const;

private:
    InputManager(); // コンストラクタを隠す
    ~InputManager() = default;

    std::vector<Uint8> previousKeyStates; // 1フレーム前のキー状態
    const Uint8* currentKeyStates;        // 現在のキー状態
    int numKeys;                          // キーの総数
};
