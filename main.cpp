#include <SDL2/SDL.h>
#include <iostream>

//マジックナンバー防止
constexpr int WiDTH = 800;
constexpr int HEIGHT= 600;
constexpr int PLAYER_SPEED = 5;
constexpr Uint32 FRAME_DELAY = 16; //60FPS

int main(int argc, char* argv[]) {
    //初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    SDL_Window* window = SDL_CreateWindow("ActionGame version0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WiDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //プレイヤーデータ
    //x,y座標とサイズ
    SDL_Rect player = {100, 400, 50, 50};

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        //イベント処理
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) isRunning = false;
        }
        //滑らかなキー入力取得
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if(state[SDL_SCANCODE_LEFT]) player.x -= PLAYER_SPEED;
        if(state[SDL_SCANCODE_RIGHT]) player.x += PLAYER_SPEED;
    

        //描画処理
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //プレイヤーの色
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &player);
        SDL_RenderPresent(renderer);
        SDL_Delay(FRAME_DELAY);
    }
    //終了処理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}