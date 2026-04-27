#include <SDL2/SDL.h>
#include <iostream>

//物理演算用の定義
const float PIXEL_PER_METER = 50.0f;               //1メートルあたりピクセル
#define KM_H(_v) ((_v) / 3.6f)  // km/hをm/sに変換するマクロ

int main(int argc, char* argv[]) {
    // SDLの初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDLの初期化に失敗しました: " << SDL_GetError() << std::endl;
        return 1;
    }

    // ウィンドウの作成（タイトルを actionman に変更）
    SDL_Window* window = SDL_CreateWindow(
        "actionman - Prototype",   // ウィンドウのタイトル
        SDL_WINDOWPOS_CENTERED,    // X座標（画面中央）
        SDL_WINDOWPOS_CENTERED,    // Y座標（画面中央）
        800, 600,                  // ウィンドウの幅と高さ
        SDL_WINDOW_SHOWN           // 表示フラグ
    );

    if (window == nullptr) {
        std::cerr << "ウィンドウの作成に失敗しました: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // レンダラーの作成
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "レンダラーの作成に失敗しました: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // --- ゲームの初期化 ---
    // プレイヤー変数
    float playerX = 400.0f;     //プレイヤーのX座標
    float playerY = 100.0f;     //プレイヤーのY座標
    float playerWidth_M = 0.5f;       //プレイヤーの幅
    float playerHeight_M = 1.0f;      //プレイヤーの高さ
    int playerWidth = (int)(playerWidth_M * PIXEL_PER_METER);   //プレイヤーの幅（ピクセル）
    int playerHeight = (int)(playerHeight_M * PIXEL_PER_METER); //プレイヤーの高さ（ピクセル）


    //物理演算用の定数（メートル単位で定義）
    const float GRAVITY_M = 9.8f * 2;                           //重力加速度
    const float MAX_FALL_SPEED_M = 20.0f;                 //最大落下速度
    const float JUMP_FORCE_M = -12.0f;                    //ジャンプ初速(m/s) 
    const float MOVE_SPEED_M = 6.0f;                      //移動速度
    const float ACCEL_X_M = 50.0f;                      //横移動加速
    const float ACCEL_A_X_M = 20.0f;                      //空中横移動加速
    const float FRICTION_X_M = 100.0f;                     //地面摩擦の減速度
    const float MAX_WALK_SPEED_M = 8.0f;                  //歩きの最大速度
    const float MAX_DASH_SPEED_M = 12.0f;                  //ダッシュの最大速度

    //内部計算用の定数（ピクセル単位に変換）
    const float GRAVITY = GRAVITY_M * PIXEL_PER_METER;  //重力加速度
    const float FALL_GRAVITY =GRAVITY * 3.0f; //落下中の重力加速度（ファストフォール）
    const float MAX_FALL_SPEED = MAX_FALL_SPEED_M * PIXEL_PER_METER; //最大落下速度
    const float JUMP_FORCE = JUMP_FORCE_M * PIXEL_PER_METER; //ジャンプ初速
    const float MOVE_SPEED = MOVE_SPEED_M * PIXEL_PER_METER; //移動速度
    const float ACCEL_X = ACCEL_X_M * PIXEL_PER_METER; //ダッシュ時の加速度
    const float ACCEL_A_X = ACCEL_A_X_M * PIXEL_PER_METER; //空中横移動加速
    const float FRICTION_X = FRICTION_X_M * PIXEL_PER_METER; //地面摩擦の減速度
    const float MAX_WALK_SPEED = MAX_WALK_SPEED_M * PIXEL_PER_METER; //歩きの最大速度
    const float MAX_DASH_SPEED = MAX_DASH_SPEED_M * PIXEL_PER_METER; //ダッシュの最大速度

    float velocityY = 0.0f;      //Y軸の速度
    float velocityX = 0.0f;      //X軸の速度
    bool isGrounded = false;      //地面にいるかどうか
    bool isMovingX = false;       //X軸に移動しているかどうか
    bool isJumpPressed = false;    //ジャンプボタンが押されているかどうか
    const float GROUND_Y = 500.0f;      //地面のY座標

    //デルタタイム用の変数
    //fps制御用の定数
    const int TARGET_FPS = 60;
    const Uint32 FRAME_DELAY = 1000 / TARGET_FPS; //フレーム
    Uint64 previousTime = SDL_GetTicks64();   //一旦60fpsにしたい
    

    // ゲームループ準備
    bool isRunning = true;
    SDL_Event event;

    // --- メインゲームループ ---
    while (isRunning) {
        // 入力・イベント処理
        while (SDL_PollEvent(&event) != 0) {
            // ウィンドウの「×」ボタンが押されたらループを抜ける
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        //デルタタイムの計算
        Uint64 frameStart = SDL_GetTicks64();
        // 経過時間をミリ秒から秒に変換
        float dt = (frameStart - previousTime) / 1000.0f;
        //処理落ち対策のための制限
        if(dt > 0.05f) dt = 0.05f;
        previousTime = frameStart;

        // 更新処理
        //キーボードの状態を取得
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        float targetMaxSpeed = MAX_WALK_SPEED; //通常の最大速度
        if (currentKeyStates[SDL_SCANCODE_LSHIFT]) {
            targetMaxSpeed = MAX_DASH_SPEED; //ダッシュの最大速度
        }

        isMovingX = false; //移動しているかどうかのフラグ
        if(isGrounded){
            //地面にいるときは通常の加速度
            if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
                velocityX += ACCEL_X * dt; //右に移動
                isMovingX = true;
            }
            if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
                velocityX -= ACCEL_X * dt; //左に移動
                isMovingX = true;
            }
        }else{
            //空中にいるときは空中加速度
            if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
                velocityX += ACCEL_A_X * dt; //右に移動
                isMovingX = true;
            }
            if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
                velocityX -= ACCEL_A_X * dt; //左に移動
                isMovingX = true;
            }
        }

        //入力がない時は摩擦で減速
        if (!isMovingX){
            if (velocityX > 0) {
                velocityX -= FRICTION_X * dt; //右に移動しているときは左に減速
                if (velocityX < 0) velocityX = 0; //速度が0を下回らないようにする
            } else if (velocityX < 0) {
                velocityX += FRICTION_X * dt; //左に移動しているときは右に減速
                if (velocityX > 0) velocityX = 0; //速度が0を上回らないようにする
            }
        }

        //速度制限
        if (velocityX > targetMaxSpeed) {
            velocityX = targetMaxSpeed;
        }
        if (velocityX < -targetMaxSpeed) {
            velocityX = -targetMaxSpeed;
        }
        
        isJumpPressed = currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_SPACE];
        
        //ジャンプ処理（初速を与える）
        if ((currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_SPACE]) && isGrounded) {
            velocityY = JUMP_FORCE;
            isGrounded = false;
        }

        //長押しジャンプ実装
        float currentGravity = GRAVITY; //通常の重力
        if(velocityY < 0.0f && !isJumpPressed){
            currentGravity = FALL_GRAVITY; //重力を強くする
        }

        // 物理演算: 半陰解放オイラー法
        velocityY += currentGravity * dt; //重力で速度更新
        if(velocityY > MAX_FALL_SPEED) velocityY = MAX_FALL_SPEED; //最大落下速度制限

        playerX += velocityX * dt; //位置更新
        playerY += velocityY * dt; //位置更新

        //地面との衝突判定
        if (playerY + playerHeight >= GROUND_Y) {
            playerY = GROUND_Y - playerHeight;
            velocityY = 0.0f;
            isGrounded = true;
        }else{
            isGrounded = false;
        }

        // 描画処理
        // 背景色設定
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        // 背景色描画
        SDL_RenderClear(renderer);

        //プレイヤー描画準備
        SDL_Rect playerRect = { (int)playerX, (int)playerY, playerWidth, playerHeight };
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255); //プレイヤーの色（赤）

        //プレイヤー描画
        SDL_RenderFillRect(renderer, &playerRect);

        // 描画したものを画面に反映
        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() -frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    // --------------------------------------------------------

    // 5. 終了処理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
